#pragma once

#include "../../Common.h"

namespace fox::anim::Experimental
{
	struct Bone
	{
		int ParentIndex;

		uint _Padding;

		void* BoneControlPtr;

		Vector3 LocalPosition;
	};
}