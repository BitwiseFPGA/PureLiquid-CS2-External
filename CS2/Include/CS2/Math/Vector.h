#pragma once
struct Vector {
	float x;
	float y;
	float z;
	constexpr Vector& operator+=(const Vector& vecBase)
	{
		this->x += vecBase.x;
		this->y += vecBase.y;
		this->z += vecBase.z;
		return *this;
	};
	Vector operator+(const Vector& vecAdd) const
	{
		return { this->x + vecAdd.x, this->y + vecAdd.y, this->z + vecAdd.z };
	};
};
using Vector3 = Vector;
