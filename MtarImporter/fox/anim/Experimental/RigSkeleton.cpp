#include "RigSkeleton.h"

namespace fox::anim::Experimental
{
	Vector3 RigSkeleton::GetLocalPosition(short index)
	{
		int sanIndex = GetSanitizedSkelIndex(index);

		return Bones[sanIndex].LocalPosition;
	}

	Vector3 RigSkeleton::GetGlobalPosition(short index)
	{
		int sanIndex = GetSanitizedSkelIndex(index);

		int adjIndex = sanIndex - UnkPoseDataSkelOffset;

		return Positions[adjIndex];
	}
	Quat RigSkeleton::GetGlobalRotation(short index)
	{
		int sanIndex = GetSanitizedSkelIndex(index);

		int adjIndex = sanIndex - UnkPoseDataSkelOffset;

		return Rotations[adjIndex];
	}

	void RigSkeleton::SetGlobalPosition(short index, Vector3 pos)
	{
		int sanIndex = GetSanitizedSkelIndex(index);

		int adjIndex = sanIndex - UnkPoseDataSkelOffset;

		Positions[adjIndex] = pos;
	}
	void RigSkeleton::SetGlobalRotation(short index, Quat rot)
	{
		int sanIndex = GetSanitizedSkelIndex(index);

		int adjIndex = sanIndex - UnkPoseDataSkelOffset;

		Rotations[adjIndex] = rot;
	}

	Vector3 RigSkeleton::GetGlobalParentPosition(short index)
	{
		int sanIndex = GetSanitizedSkelIndex(index);

		int parentIndex = Bones[sanIndex].ParentIndex - UnkPoseDataSkelOffset;

		return parentIndex < 0 ? Vector3{} : Positions[parentIndex];
	}
	Quat RigSkeleton::GetGlobalParentRotation(short index)
	{
		int sanIndex = GetSanitizedSkelIndex(index);

		int parentIndex = Bones[sanIndex].ParentIndex - UnkPoseDataSkelOffset;

		return parentIndex < 0 ? Quat::identity() : Rotations[parentIndex];
	}

	int RigSkeleton::GetSanitizedSkelIndex(short index)
	{
		if (index < 0)
			return -1;
		else if (BoneIndexRemaps != (short*)0x0)
			return BoneIndexRemaps[index];

		return index;
	}

	bool RigSkeleton::GetUnknownBool()
	{
		return UnknownBool;
	}
}