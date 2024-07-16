#pragma once

#include "../../Common.h"
#include "Bone.h"

#include <tuple>

namespace fox::anim::Experimental
{
	struct RigSkeleton
	{
		Vector3 GetLocalPosition(short index);

		Vector3 GetGlobalPosition(short index);
		Quat GetGlobalRotation(short index);

		void SetGlobalPosition(short index, Vector3 pos);
		void SetGlobalRotation(short index, Quat rot);

		Vector3 GetGlobalParentPosition(short index);
		Quat GetGlobalParentRotation(short index);

		bool GetUnknownBool();

	private:

		int GetSanitizedSkelIndex(short index);

	private:

		uint Bitfield;

		ubyte _Padding0[44];

		Bone* Bones;

		short* BoneIndexRemaps;

		bool UnknownBool;

		ubyte _Padding1[7];

		uint BoneCount;

		uint _Padding2;

		Quat* Rotations;

		Vector3* Positions;

		ulong _Padding3;

		int UnkPoseDataSkelOffset;

		uint _Padding4;
	};
}