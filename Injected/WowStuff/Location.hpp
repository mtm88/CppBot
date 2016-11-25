#pragma once
#include <math.h>
#include <string>
#include <d3dx9math.h>

struct Location : D3DXVECTOR3
{
	Location() :D3DXVECTOR3(0, 0, 0) {}

	Location(D3DXVECTOR3& v) :D3DXVECTOR3(v) {}

	Location(float x, float y, float z)	:D3DXVECTOR3(x, y, z) {}

	bool Invalid() const
	{
		return (!x && !y && !z);
	}

	float DistanceTo(const Location& loc) const
	{
		return sqrt(pow(x - loc.x, 2) + pow(y - loc.y, 2) + pow(z - loc.z, 2));
	}

	float Distance2D(const Location& loc) const
	{
		return sqrt(pow(x - loc.x, 2) + pow(y - loc.y, 2));
	}

	float Length() const
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	float Angle() const
	{
		return atan2(y, x);
	}

	std::string ToString() const
	{
		return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]";
	}
};

inline Location Normalize(Location const& v)
{
	Location vec;
	float length = v.Length();
	if (length != 0)
	{
		vec.x = v.x / length;
		vec.y = v.y / length;
		vec.z = v.z / length;
	}
	return vec;
}