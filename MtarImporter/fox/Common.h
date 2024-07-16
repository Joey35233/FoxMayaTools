#pragma once

#include <stdint.h>

typedef uint8_t ubyte;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef uint64_t ulong;

struct Vector2
{
	float x;
	float y;
};

//typedef struct
//{
//	half x;
//	half y;
//} VectorH2 <read = Str("(%s, %s)", ReadHalf(x), ReadHalf(y))>;

struct Vector3
{
	float x;
	float y;
	float z;
	float w;

	inline Vector3()
	{
		x = 0;
		y = 0;
		z = 0;

		w = 0;
	}

	inline Vector3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;

		w = 0;
	}

	explicit inline Vector3(float scalar)
	{
		x = scalar;
		y = scalar;
		z = scalar;

		w = 0;
	}

	static inline const float length(const Vector3& vec)
	{
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	}

	static inline const Vector3 normalize(const Vector3& vec)
	{
		return vec / Vector3::length(vec);
	}

	inline const Vector3 operator *(float scale) const
	{
		return Vector3{ x * scale, y * scale, z * scale };
	}

	inline const Vector3 operator /(float div) const
	{
		return Vector3{ x / div, y / div, z / div };
	}

	inline const Vector3 operator +(const Vector3& vec) const
	{
		return Vector3{ x + vec.x, y + vec.y, z + vec.z };
	}

	inline const Vector3 operator -(const Vector3& vec) const
	{
		return Vector3{ x - vec.x, y - vec.y, z - vec.z };
	}
};

struct SmallVector3
{
	float x;
	float y;
	float z;
};

//typedef struct
//{
//	half x;
//	half y;
//	half z;
//} VectorH3 <read = Str("(%s, %s, %s)", ReadHalf(x), ReadHalf(y), ReadHalf(z))>;

struct Vector3P
{
	float x;
	float y;
	float z;
	float _Padding;

	Vector3P()
	{
		x = 0;
		y = 0;
		z = 0;

		_Padding = 1.0f;
	}
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

//typedef struct
//{
//	half x;
//	half y;
//	half z;
//	half w;
//} VectorH4 <read = Str("(%s, %s, %s, %s)", ReadHalf(x), ReadHalf(y), ReadHalf(z), ReadHalf(w))>;

//typedef struct
//{
//	ubyte x <read = ((float)this / 0xFF)>;
//	ubyte y <read = ((float)this / 0xFF)>;
//	ubyte z <read = ((float)this / 0xFF)>;
//	ubyte w <read = ((float)this / 0xFF)>;
//} VectorB4 <read = Str("(%g, %g, %g, %g)", (float)x / 0xFF, (float)y / 0xFF, (float)z / 0xFF, (float)w / 0xFF)>;

struct Quat
{
	float x;
	float y;
	float z;
	float w;

	static inline const Quat Quat::identity()
	{
		return Quat{ 0, 0, 0, 1.0f };
	}

	static inline const Vector3 rotate(const Quat& unitQuat, const Vector3& vec)
	{
		return Vector3{};
	}
};

//typedef struct
//{
//	half x;
//	half y;
//	half z;
//	half w;
//} QuaternionH <read = Str("(%s, %s, %s, %s)", ReadHalf(x), ReadHalf(y), ReadHalf(z), ReadHalf(w))>;

namespace fox
{
	typedef uint StrCode32;
	typedef ulong StrCode;
	typedef ulong PathCode64;
}