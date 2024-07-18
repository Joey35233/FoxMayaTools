#include "TrackHeader.h"

#include <limits>
#include <cmath>

namespace fox::anim
{
	constexpr float Pi = 3.141592653589793238462643383279502884197169399375105820974944f;

	ushort ReadUnalignedBits(ushort const* buffer, ulong& inoutBitstreamPos, uint bitSize)
	{
		ulong baseIndex = inoutBitstreamPos >> 4; // Bit pos to ushort pos
		uint startPosInBits = inoutBitstreamPos - (baseIndex << 4); // [0, 16)

		uint lowerIndex = baseIndex;
		uint lowerShortBitOffset = startPosInBits;
		uint lowerShortBitCount = bitSize > (16 - lowerShortBitOffset) ? 16 - lowerShortBitOffset : bitSize;
		
		uint upperIndex = baseIndex + (lowerShortBitCount < bitSize);
		uint upperShortBitOffset = 0;
		uint upperShortBitCount = bitSize - lowerShortBitCount;

		ushort mask;

		mask = (ushort)-1;
		mask >>= (16 - lowerShortBitCount);
		ushort lowerValue = buffer[lowerIndex] >> lowerShortBitOffset;
		lowerValue &= mask;

		mask = (ushort)-1;
		mask >>= (16 - upperShortBitCount);
		ushort upperValue = buffer[upperIndex] >> upperShortBitOffset;
		upperValue &= mask;

		ushort value = lowerValue | (upperValue << lowerShortBitCount);

		inoutBitstreamPos += bitSize;

		return value;
	}

	Quat ReadUnalignedQuaternion(ushort const* buffer, ulong& inoutBitstreamPos, uint bitSize)
	{
		ushort denominator = (1 << bitSize) - 1;

		float halfTheta = (float)ReadUnalignedBits(buffer, inoutBitstreamPos, bitSize) / denominator * Pi / 2; // Map to half-angle range for quaternion
		
		
		float X = (float)ReadUnalignedBits(buffer, inoutBitstreamPos, bitSize) / denominator;
		float Y = (float)ReadUnalignedBits(buffer, inoutBitstreamPos, bitSize) / denominator;
		float Z = 1.0 - X - Y;
		
		float len = sqrtf(X*X + Y*Y + Z*Z);

		X /= len;
		Y /= len;
		Z /= len;

		if (ReadUnalignedBits(buffer, inoutBitstreamPos, 1) > 0)
			X = -X;
		if (ReadUnalignedBits(buffer, inoutBitstreamPos, 1) > 0)
			Y = -Y;
		if (ReadUnalignedBits(buffer, inoutBitstreamPos, 1) > 0)
			Z = -Z;

		//X = -X;
		//halfTheta = -halfTheta;

		float a = sinf(halfTheta);
		float b = cosf(halfTheta);

		return Quat{ a*X, a*Y, a*Z, b };
	}

	TrackData* TrackUnit::GetData(uint segmentIndex)
	{
		if (segmentIndex >= SegmentCount)
			return nullptr;

		TrackData* trackData = (TrackData*)((ubyte*)this + sizeof(TrackUnit));

		uint i = 0;
		while (i++ != segmentIndex)
			trackData = (TrackData*)((ubyte*)trackData + trackData->NextEntryOffset);

		return trackData;
	}

	TrackUnit* TrackHeader::GetUnit(uint channelIndex)
	{
		if (channelIndex >= ChannelCount)
			return nullptr;

		uint* unitOffsets = (uint*)((ubyte*)this + sizeof(TrackHeader));

		return (TrackUnit*)((ubyte*)this + unitOffsets[channelIndex]);
	}
}