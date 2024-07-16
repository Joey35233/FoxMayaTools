#pragma once

#include "../../Common.h"

namespace fox::anim::Experimental
{
	struct RigUnitDef
	{
		uint Type;
		short TrackCount;
		short BoneCount;
		short ParentBoneIndex;
		short ParentUnitIndex;

		uint _Padding;
	};
}