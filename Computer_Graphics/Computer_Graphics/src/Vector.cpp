#include <pch.h>
#include "Vector.h"

namespace vlr 
{
	Vector::Vector(void)
		: x(0), y(0), z(0)
	{}
	Vector::Vector(float x, float y, float z)
		: x(x), y(y), z(z)
	{}
	Vector::~Vector()
	{}

	float Vector::Magnitude()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}
	Vector Vector::Normalized()
	{
		return Vector(x, y, z) * (1.0f / this->Magnitude());
	}
	bool Vector::IsZero()
	{
		return (this->Magnitude() - 0.0f < 1e-5f) ? true : false;
	}
	bool Vector::IsAlign(Vector& vec)
	{
		return ((this->Dot(vec) / (this->Magnitude() * vec.Magnitude())) - 1.0f < 1e-5f) ? true : false;
	}
	bool Vector::IsPerpendicular(Vector& vec)
	{
		return ((this->Dot(vec) / (this->Magnitude() * vec.Magnitude())) - 0.0f < 1e-5f) ? true : false;
	}

	float Vector::Dot(Vector const& vec)
	{
		return (x * vec.x) + (y * vec.y) + (z * vec.z);
	}
	Vector Vector::Cross(Vector const& vec)
	{
		return Vector();
	}

	Vector Vector::operator+(Vector const& vec) 
	{
		Vector result;
		result.x = x + vec.x;
		result.y = y + vec.y;
		result.z = z + vec.z;
		return result;
	}
	Vector Vector::operator-(Vector const& vec) 
	{
		Vector result;
		result.x = x - vec.x;
		result.y = y - vec.y;
		result.z = z - vec.z;
		return result;
	}
	Vector Vector::operator*(float n)
	{
		Vector result;
		result.x = x * n;
		result.y = y * n;
		result.z = z * n;
		return result;
	}
	Vector Vector::operator*(Vector const& vec) 
	{
		Vector result;
		result.x = x * vec.x;
		result.y = y * vec.y;
		result.z = z * vec.z;
		return result;
	}


	Vector2D::Vector2D(void)
		: Vector()
	{}
	Vector2D::Vector2D(float x, float y)
		: Vector(x, y, 0)
	{}
	Vector2D::~Vector2D()
	{}


	Vector3D::Vector3D(void)
		: Vector()
	{}
	Vector3D::Vector3D(float x, float y, float z)
		: Vector(x, y, z)
	{}
	Vector3D::~Vector3D()
	{}
}