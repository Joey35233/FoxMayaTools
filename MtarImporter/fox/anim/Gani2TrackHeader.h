#pragma once

#include "../Common.h"
#include "TrackHeader.h"

namespace fox::anim
{
	struct Gani2Param
	{
		StrCode32 Name;
		float Value;
	};

	struct Gani2TrackData
	{
		uint ComponentBitSize : 8;

		uint DataOffset : 24;
	};

	struct Gani2TrackHeader
	{
		uint FrameCount;

		ubyte Padding0;

		ubyte ParamCount;

		ushort Padding1;

		bool TryGetParam(StrCode32 name, float& outValue);

		TrackUnitFlags GetUnitFlags(uint index);
	};
}