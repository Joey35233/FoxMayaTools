#include "Gani2TrackHeader.h"

namespace fox::anim
{
	bool Gani2TrackHeader::TryGetParam(StrCode32 name, float& outValue)
	{
		for (uint i = 0; i < this->ParamCount; i++)
		{
			Gani2Param* param = (Gani2Param*)((ubyte*)this + sizeof(Gani2TrackHeader)) + i;
			if (param->Name == name)
			{
				outValue = param->Value;
				return true;
			}
		}

		outValue = 0.0f;
		return false;
	}

	TrackUnitFlags Gani2TrackHeader::GetUnitFlags(uint index)
	{
		return ((TrackUnitFlags*)((ubyte*)this + sizeof(Gani2TrackHeader) + ParamCount * sizeof(Gani2Param)))[index];
	}
}