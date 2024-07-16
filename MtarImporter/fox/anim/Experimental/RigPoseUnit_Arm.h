#pragma once

#include "../../Common.h"

namespace fox::anim::Experimental
{
	struct RigPoseUnit_Arm
	{
		Quat ShoulderRot;
		Vector3 EffectorPos;
		Quat UnknownRot;
		Quat RootRot;
		Vector3 RootPos;
		float ChainVecLength;
		uint Flag;

		ulong _Padding;
	};
}