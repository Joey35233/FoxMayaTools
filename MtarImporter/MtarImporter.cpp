//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// Produces the dependency graph node "yTwist".
//
// This plug-in demonstrates how to create a user-defined deformer.
// A deformer is a node which takes any number of input geometries, deforms
// them, and places the output into the output geometry attribute.
// This example plug-in defines a new deformer node that twists the deformed
// vertices of the input around the y-axis.
//
// To use this node: 
//	(1) Create a sphere or some other object. 
//	(2) Select the object. 
//	(3) Type: "deformer -type yTwist". 
//	(4) Bring up the channel box. 
//	(5) Select the yTwist input. 
//	(6) Change the Angle value of the yTwist input in the channel box. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxFileTranslator.h>

#include <maya/MTime.h>
#include <maya/MDistance.h>
#include <maya/MAngle.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>

#include <maya/MDagModifier.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnTransform.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MPlug.h>

#include <fstream>
#include <filesystem>

#include "fox/Common.h"
#include "fox/anim/MtarHeader.h"
#include "fox/anim/Gani2TrackHeader.h"
#include "MtarImporter.h"

animImport::animImport()
	: MPxFileTranslator()
{
}

animImport::~animImport()
{
}

void* animImport::creator()
{
	return new animImport();
}

const char* UnhashRig(fox::StrCode32 trackHash)
{
	switch (trackHash)
	{
	case 3552837520:
		return "Root";
	case 2832076631:
		return "Waist";
	case 538406145:
		return "Spine";
	case 1382944449:
		return "Chest";
	case 12750096:
		return "Neck";
	case 1833209204:
		return "Head";
	case 4069048318:
		return "LArm";
	case 1626172505:
		return "LHand";
	case 2063241216:
		return "RArm";
	case 4246335734:
		return "RHand";
	case 1587345382:
		return "LLeg";
	case 2318116707:
		return "LFoot";
	case 1917416821:
		return "RLeg";
	case 3730058848:
		return "RFoot";
	case 657792596:
		return "LToe";
	case 2688182121:
		return "RToe";
	case 3930921867:
		return "LFingers";
	case 2376760760:
		return "RFingers";
	default:
		return nullptr;
	}
}

MStatus animImport::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
{
	MString fileName = file.expandedFullName();

	std::unique_ptr<ubyte> fileData;
	{
		std::ifstream mtarFile = std::ifstream(fileName.asChar());
		size_t fileSize = std::filesystem::file_size(fileName.asChar());

		fileData = std::unique_ptr<ubyte>(new ubyte[fileSize]);

		mtarFile.read((char*)fileData.get(), fileSize);
	}

	if (mode == kImportAccessMode) 
	{
		fox::anim::MtarHeader* header = (fox::anim::MtarHeader*)fileData.get();

		MDagModifier dagModifier;

		if ((ushort)header->Flags & (ushort)fox::anim::MtarHeader::MtarFlags::New)
		{
			fox::anim::TrackHeader* layoutTrack = header->GetLayoutTrack();
			if (layoutTrack == nullptr)
				return MS::kFailure;

			fox::anim::Mtar2FileHeader* fileHeader = header->GetFile2Headers();
			fox::anim::Gani2TrackHeader* trackHeader = (fox::anim::Gani2TrackHeader*)((ubyte*)header + fileHeader->DataOffset);
			fox::anim::Gani2TrackData* gani2TrackDataArray;
			{ 
				ubyte* paramEndPtr = (ubyte*)trackHeader + sizeof(fox::anim::Gani2TrackHeader) + trackHeader->ParamCount * sizeof(fox::anim::Gani2Param) + header->TrackCount * sizeof(fox::anim::TrackUnitFlags);
				
				ulong intPtr = (ulong)paramEndPtr;
				intPtr += sizeof(fox::anim::Gani2TrackData) - 1;
				intPtr &= -(int64_t)sizeof(fox::anim::Gani2TrackData);
				gani2TrackDataArray = (fox::anim::Gani2TrackData*)intPtr;
			}

			MStatus status;
			uint absTrackDataIndex = 0;
			for (uint i = 0; i < layoutTrack->ChannelCount; i++)
			{
				fox::anim::TrackUnit* trackUnit = layoutTrack->GetUnit(i);

				for (uint j = 0; j < trackUnit->SegmentCount; j++, absTrackDataIndex++)
				{
					// Parse TrackUnit
					fox::anim::TrackData* trackData = trackUnit->GetData(j);
					fox::anim::Gani2TrackData* gani2TrackData = gani2TrackDataArray + absTrackDataIndex;

					// Create RigDef attribute locator
					MObject	trackDataLocator = dagModifier.createNode("locator", MObject::kNullObj, &status);
					auto rigTryUnhash = UnhashRig(trackUnit->Name);
					std::string trackDataName = "TK_" + (rigTryUnhash == nullptr ? std::to_string(trackUnit->Name) : rigTryUnhash) + std::string("_") + std::to_string(j) + ((trackData->Type == fox::anim::TrackType::Quat || trackData->Type == fox::anim::TrackType::QuatDiff) ? "_Quat" : "_Vec");
					dagModifier.renameNode(trackDataLocator, trackDataName.c_str());

					ushort* trackDataBlob = (ushort*)((ubyte*)gani2TrackData + gani2TrackData->DataOffset);

					// Animate
					MFnDagNode node = MFnDagNode(trackDataLocator, &status);
					MPlug rotationInterpolation = node.findPlug("rotationInterpolation", &status);
					status = rotationInterpolation.setString("quaternionSlerp");

					ulong time = 0;
					switch (trackData->Type)
					{
					case fox::anim::TrackType::Quat:
						{
							// X
							MObject attributeX = node.attribute("rotateX");
							MFnAnimCurve curveX;
							curveX.create(node.object(), attributeX, NULL, &status);
							status = curveX.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveX.setPostInfinityType(MFnAnimCurve::kConstant);
							// Y
							MObject attributeY = node.attribute("rotateY");
							MFnAnimCurve curveY;
							curveY.create(node.object(), attributeY, NULL, &status);
							status = curveY.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveY.setPostInfinityType(MFnAnimCurve::kConstant);
							// Z
							MObject attributeZ = node.attribute("rotateZ");
							MFnAnimCurve curveZ;
							curveZ.create(node.object(), attributeZ, NULL, &status);
							status = curveZ.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveZ.setPostInfinityType(MFnAnimCurve::kConstant);

							ulong inoutBitstreamPos = 0;

							MTime keyTime = MTime(time, MTime::k59_94FPS );
							Quat startKeyFloatQuat = fox::anim::ReadUnalignedQuaternion(trackDataBlob, inoutBitstreamPos, gani2TrackData->ComponentBitSize);

							MEulerRotation startKeyEuler = MQuaternion(startKeyFloatQuat.x, startKeyFloatQuat.y, startKeyFloatQuat.z, startKeyFloatQuat.w).asEulerRotation();
							status = curveX.addKeyframe(keyTime, startKeyEuler.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveY.addKeyframe(keyTime, startKeyEuler.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveZ.addKeyframe(keyTime, startKeyEuler.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);

							if (((ubyte)trackHeader->GetUnitFlags(i) & (ubyte)fox::anim::TrackUnitFlags::NoFrames) == 0)
							{
								while (time < trackHeader->FrameCount)
								{
									ubyte frameCount = (ubyte)fox::anim::ReadUnalignedBits(trackDataBlob, inoutBitstreamPos, 8);
									time += frameCount;

									Quat valueFloatQuat = fox::anim::ReadUnalignedQuaternion(trackDataBlob, inoutBitstreamPos, gani2TrackData->ComponentBitSize);
									MEulerRotation value = MQuaternion(valueFloatQuat.x, valueFloatQuat.y, valueFloatQuat.z, valueFloatQuat.w).asEulerRotation();

									keyTime = MTime(time, MTime::k59_94FPS);
									status = curveX.addKeyframe(keyTime, value.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveY.addKeyframe(keyTime, value.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveZ.addKeyframe(keyTime, value.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
								}
							}
						}
						break;
					case fox::anim::TrackType::Float:
						return MS::kNotImplemented;
						break;
					case fox::anim::TrackType::Vector2:
						return MS::kNotImplemented;
						break;
					case fox::anim::TrackType::Vector3:
						{
							// X
							MObject attributeX = node.attribute("translateX");
							MFnAnimCurve curveX;
							curveX.create(node.object(), attributeX, NULL, &status);
							status = curveX.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveX.setPostInfinityType(MFnAnimCurve::kConstant);
							// Y
							MObject attributeY = node.attribute("translateY");
							MFnAnimCurve curveY;
							curveY.create(node.object(), attributeY, NULL, &status);
							status = curveY.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveY.setPostInfinityType(MFnAnimCurve::kConstant);
							// Z
							MObject attributeZ = node.attribute("translateZ");
							MFnAnimCurve curveZ;
							curveZ.create(node.object(), attributeZ, NULL, &status);
							status = curveZ.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveZ.setPostInfinityType(MFnAnimCurve::kConstant);

							ubyte* dataBlob = (ubyte*)trackDataBlob;

							MTime keyTime = MTime(time, MTime::k59_94FPS);
							SmallVector3 startKey = *(SmallVector3*)dataBlob; dataBlob += sizeof(SmallVector3);
							status = curveX.addKeyframe(keyTime, startKey.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveY.addKeyframe(keyTime, startKey.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveZ.addKeyframe(keyTime, startKey.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);

							if (((ubyte)trackHeader->GetUnitFlags(i) & (ubyte)fox::anim::TrackUnitFlags::NoFrames) == 0)
							{
								while (time < trackHeader->FrameCount)
								{
									ubyte frameCount = *dataBlob; dataBlob++;
									time += frameCount;

									SmallVector3 value = *(SmallVector3*)dataBlob; dataBlob += sizeof(SmallVector3);

									keyTime = MTime(time, MTime::k59_94FPS);
									status = curveX.addKeyframe(keyTime, value.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveY.addKeyframe(keyTime, value.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveZ.addKeyframe(keyTime, value.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
								}
							}
						}
						break;
					case fox::anim::TrackType::Vector4:
						return MS::kNotImplemented;
						break;
					case fox::anim::TrackType::QuatDiff:
						{
							// X
							MObject attributeX = node.attribute("rotateX");
							MFnAnimCurve curveX;
							curveX.create(node.object(), attributeX, NULL, &status);
							status = curveX.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveX.setPostInfinityType(MFnAnimCurve::kConstant);
							// Y
							MObject attributeY = node.attribute("rotateY");
							MFnAnimCurve curveY;
							curveY.create(node.object(), attributeY, NULL, &status);
							status = curveY.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveY.setPostInfinityType(MFnAnimCurve::kConstant);
							// Z
							MObject attributeZ = node.attribute("rotateZ");
							MFnAnimCurve curveZ;
							curveZ.create(node.object(), attributeZ, NULL, &status);
							status = curveZ.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveZ.setPostInfinityType(MFnAnimCurve::kConstant);

							MEulerRotation runningRot = MEulerRotation::identity;
							ulong inoutBitstreamPos = 0;

							MTime keyTime = MTime(time, MTime::k59_94FPS);
							Quat startKeyFloatQuat = fox::anim::ReadUnalignedQuaternion(trackDataBlob, inoutBitstreamPos, gani2TrackData->ComponentBitSize);
							runningRot = MQuaternion(startKeyFloatQuat.x, startKeyFloatQuat.y, startKeyFloatQuat.z, startKeyFloatQuat.w);

							status = curveX.addKeyframe(keyTime, runningRot.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveY.addKeyframe(keyTime, runningRot.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveZ.addKeyframe(keyTime, runningRot.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);

							if (((ubyte)trackHeader->GetUnitFlags(i) & (ubyte)fox::anim::TrackUnitFlags::NoFrames) == 0)
							{
								while (time < trackHeader->FrameCount)
								{
									ubyte frameCount = (ubyte)fox::anim::ReadUnalignedBits(trackDataBlob, inoutBitstreamPos, 8);
									time += frameCount;

									Quat deltaFloatQuat = fox::anim::ReadUnalignedQuaternion(trackDataBlob, inoutBitstreamPos, gani2TrackData->ComponentBitSize);
									runningRot *= MQuaternion(deltaFloatQuat.x, deltaFloatQuat.y, deltaFloatQuat.z, deltaFloatQuat.w);

									keyTime = MTime(time, MTime::k59_94FPS);
									status = curveX.addKeyframe(keyTime, runningRot.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveY.addKeyframe(keyTime, runningRot.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveZ.addKeyframe(keyTime, runningRot.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
								}
							}
						}
						break;
					case fox::anim::TrackType::RootPos:
						{
							// X
							MObject attributeX = node.attribute("translateX");
							MFnAnimCurve curveX;
							curveX.create(node.object(), attributeX, NULL, &status);
							status = curveX.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveX.setPostInfinityType(MFnAnimCurve::kConstant);
							// Y
							MObject attributeY = node.attribute("translateY");
							MFnAnimCurve curveY;
							curveY.create(node.object(), attributeY, NULL, &status);
							status = curveY.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveY.setPostInfinityType(MFnAnimCurve::kConstant);
							// Z
							MObject attributeZ = node.attribute("translateZ");
							MFnAnimCurve curveZ;
							curveZ.create(node.object(), attributeZ, NULL, &status);
							status = curveZ.setPreInfinityType(MFnAnimCurve::kConstant);
							status = curveZ.setPostInfinityType(MFnAnimCurve::kConstant);

							ubyte* dataBlob = (ubyte*)trackDataBlob;

							MTime keyTime = MTime(time, MTime::k59_94FPS );
							SmallVector3 startKey = *(SmallVector3*)dataBlob; dataBlob += sizeof(SmallVector3);
							status = curveX.addKeyframe(keyTime, startKey.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveY.addKeyframe(keyTime, startKey.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
							status = curveZ.addKeyframe(keyTime, startKey.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);

							if (((ubyte)trackHeader->GetUnitFlags(i) & (ubyte)fox::anim::TrackUnitFlags::NoFrames) == 0)
							{
								while (time < trackHeader->FrameCount)
								{
									ubyte frameCount = *dataBlob; dataBlob++;
									time += frameCount;

									SmallVector3 value = *(SmallVector3*)dataBlob; dataBlob += sizeof(SmallVector3);

									keyTime = MTime(time, MTime::k59_94FPS);
									status = curveX.addKeyframe(keyTime, value.x, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveY.addKeyframe(keyTime, value.y, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
									status = curveZ.addKeyframe(keyTime, value.z, MFnAnimCurve::TangentType::kTangentLinear, MFnAnimCurve::TangentType::kTangentLinear);
								}
							}
						}
						break;
					}
				}
			}

			status = dagModifier.doIt();

			printf("Returning success.\n");

			return MS::kSuccess;
		}
		else
		{
			return MS::kNotImplemented;
		}
	}

	return MS::kFailure;
}

bool animImport::haveReadMethod() const
{
	return true;
}

bool animImport::haveWriteMethod() const
{
	return false;
}

bool animImport::canBeOpened() const
{
	return false;
}

MString animImport::defaultExtension() const
{
	return MString("mtar");
}

MPxFileTranslator::MFileKind animImport::identifyFile(const MFileObject& fileName, const char* buffer, short size) const
{
	const char* name = fileName.resolvedName().asChar();
	int nameLength = (int)strlen(name);

	if ((nameLength > 5) && !strcasecmp(name + nameLength - 5, ".mtar")) {
		return kIsMyFileType;
	}

	if (size > 4 && *(uint*)buffer == 201403250) {
		return kIsMyFileType;
	}

	return kNotMyFileType;
}

MStatus initializePlugin(MObject obj)
{
	MStatus stat = MS::kFailure;
	MFnPlugin impPlugIn(obj, "FoxKit", "0.1", "Any");

	stat = impPlugIn.registerFileTranslator
	(
		"MtarImporter", 
		"none",
		animImport::creator
	);

	if (stat != MS::kSuccess)
		return stat;

	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat = MS::kFailure;

	MFnPlugin impPlugIn(obj);
	stat = impPlugIn.deregisterFileTranslator("MtarImporter");

	if (stat != MS::kSuccess)
		return stat;

	return stat;
}