#include "Mtar2MiniDataNode.h"

namespace fox::anim
{
	Mtar2MiniDataNode* Mtar2MiniDataNode::Find(StrCode32 name)
	{
		if (Name == name)
			return this;

		if (NextNodeOffset == 0)
			return nullptr;

		return ((Mtar2MiniDataNode*)((ubyte*)this + NextNodeOffset))->Find(name);
	}
}