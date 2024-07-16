#pragma once

#include "../Common.h"
#include <cstring>

namespace fox::anim
{
	struct AnimHalf
	{
		ushort value;

		float ToFloat()
		{
			uint sign = (value & 0x8000) >> 8;
			uint exponent = value & 0x7C00;
			if (exponent > 0)
				exponent = (exponent + 0x1DC00) >> 3;
			uint mantissa = (value & 1023) << 5;

			uint outBuffer = 0;
			outBuffer |= (uint)(mantissa & 0xFF) << 8;
			outBuffer |= (uint)(exponent & 0x80 | (mantissa >> 8) & 0xFF) << 16;
			outBuffer |= (uint)(sign | exponent >> 8 & 0xFF) << 24;

			float outFloat = 0;
			memcpy(&outFloat, &outBuffer, 4);
			return outFloat;
		}
	};

	struct VectorAnimH3
	{
		AnimHalf x;
		AnimHalf y;
		AnimHalf z;
	};

	struct VectorAnimH4
	{
		AnimHalf x;
		AnimHalf y;
		AnimHalf z;
		AnimHalf w;
	};

	struct QuatAnim12
	{
		ushort Theta : 12;
		ushort Y : 12;
		ushort Z : 12;
		ushort X_Sign : 1;
		ushort Y_Sign : 1;
		ushort Z_Sign : 1;
	};

	struct QuatAnim13
	{
		ushort Theta : 13;
		ushort Y : 13;
		ushort Z : 13;
		ushort X_Sign : 1;
		ushort Y_Sign : 1;
		ushort Z_Sign : 1;
	};

	struct QuatAnim15
	{
		ushort Theta : 15;
		ushort Y : 15;
		ushort Z : 15;
		ushort X_Sign : 1;
		ushort Y_Sign : 1;
		ushort Z_Sign : 1;
	};

	ushort ReadUnalignedBits(ushort const* buffer, ulong& inoutBitstreamPos, uint bitSize);
	Quat ReadUnalignedQuaternion(ushort const* buffer, ulong& inoutBitstreamPos, uint bitSize);

	enum class TrackType : ubyte
	{
		Quat = 0,
		Float = 1,
		Vector2 = 2,
		Vector3 = 3,
		Vector4 = 4,
		QuatDiff = 5,
		RootPos = 6,
	};

	struct TrackData
	{
		int DataOffset;

		short MotionSegmentId;

		TrackType Type : 4;

		ubyte NextEntryOffset : 4;

		ubyte ComponentBitSize;
	};

	enum class TrackUnitFlags : ubyte
	{
		Loop = 1 << 0,
		HermiteVectorInterpolation = 1 << 1,
		NoFrames = 1 << 2,
	};

	struct TrackUnit
	{
		StrCode32 Name;

		ubyte SegmentCount;

		TrackUnitFlags Flags;

		ushort Padding;

		TrackData* GetData(uint segmentIndex);
	};

	struct TrackHeader
	{
		uint ChannelCount;

		uint TrackCount;

		ushort HeaderIndex; // Used in SAND. In GANI, 0 for FK, 1 for IK

		ubyte Unknown;

		ubyte Flag;

		uint FrameCount;

		uint Unknown1;

		TrackUnit* GetUnit(uint channelIndex);
	};
}