#pragma once

#include "../Common.h"

namespace fox::anim
{
	struct Mtar2MiniDataNode
	{
		StrCode32 Name;

		uint DataSize;

		uint NextNodeOffset;

		uint Padding;

		Mtar2MiniDataNode* Find(StrCode32 name);
	};

	struct MtarMTPBoneAssociation
	{
		StrCode32 MTPName;

		StrCode32 BoneName;
	};
}