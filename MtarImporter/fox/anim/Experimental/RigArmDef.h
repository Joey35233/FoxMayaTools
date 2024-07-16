#pragma once

#include "../../Common.h"

#include "RigUnitDef.h"
#include "RigSkeleton.h"
#include "RigPoseUnit_Arm.h"

namespace fox::anim::Experimental
{
	struct RigArmDef
	{
		RigUnitDef Base;

		ubyte _Padding0[16];

		Vector3 ChainPlaneNormal;

		short ChainIndexA;
		short ChainIndexB;
		short ChainIndexC;

		short SegmentIndexA;
		short SegmentIndexB;
		short SegmentIndexC;

		short EffectorSkelIndex;

		ushort _Padding1;

	private:
		void AnimateShoulder(Vector3* outChainA_gp, Quat* outChainARot, Vector3* outChainB_gp, RigSkeleton* pose, RigPoseUnit_Arm* poseUnit);

		void PoseToSkeleton(RigSkeleton* skeleton, RigPoseUnit_Arm* poseUnit);
	};
}