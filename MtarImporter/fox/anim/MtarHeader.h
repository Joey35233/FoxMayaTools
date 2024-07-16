#pragma once

#include "../Common.h"
#include "TrackHeader.h"
#include "Mtar2MiniDataNode.h"

namespace fox::anim
{
	struct MtarFileHeader
	{
		PathCode64 Path;
		uint DataOffset;
		ushort DataSize;
		ushort Unknown;
	};

	struct Mtar2FileHeader
	{
		PathCode64 Path;
		uint DataOffset;
		ushort DataSize;

		ushort FkDataOffset;
		uint FkDataSize;

		uint Padding0;
		uint MotionEventsOffset;

		uint Padding1;
	};

	struct MtarHeader
	{
		uint Version;

		uint FileCount;

		ushort TrackCount; // Same as FRIG's RigUnit count

		ushort SegmentCount; // Same as FRIG's count

		ushort ShaderNodeCount; // Shows up in "_facial" - seems to be the number of SHADER nodes per animation

		ushort Unknown1; // Shows up in "_facial" normally (always?) as 0x1

		enum class UnknownFlags : ushort
		{
			Unknown0x1 = 0x1,
			Unknown0x2 = 0x2,
			Unknown0x4 = 0x4,
			Unknown0x8 = 0x8,
			Unknown0x10 = 0x10,
		} UnknownFlags;

		enum class MtarFlags : ushort
		{
			New = 0x1000,

			MtpIsBone = 0x4000,
		} Flags;

		uint CommonInfoOffset;

		ulong Padding;

		MtarFileHeader* GetFileHeaders()
		{
			return (MtarFileHeader*)((ubyte*)this + sizeof(MtarHeader));
		}

		Mtar2FileHeader* GetFile2Headers()
		{
			return (Mtar2FileHeader*)((ubyte*)this + sizeof(MtarHeader));
		}

		TrackHeader* GetLayoutTrack()
		{
			if (((ushort)Flags & (uint)MtarFlags::New) == 0)
				return nullptr;

			Mtar2MiniDataNode* node = (Mtar2MiniDataNode*)((ubyte*)this + CommonInfoOffset);
			
			Mtar2MiniDataNode* trackNode = node->Find(1337830127);

			if (trackNode == nullptr)
				return nullptr;
			else
				return (TrackHeader*)((ubyte*)trackNode + sizeof(Mtar2MiniDataNode));
		}
	};
}