#pragma once

#include "VLRCore.h"

namespace vlr
{
	class Vector 
	{
	public:
		Vector(void);
		Vector(float x, float y, float z);
		virtual ~Vector();

		float Magnitude();
		Vector Normalized();
		bool IsZero();
		bool IsAlign(Vector& vec);
		bool IsPerpendicular(Vector& vec);

		float Dot(Vector const& vec);
		Vector Cross(Vector const& vec);

		Vector operator+(Vector const& vec);
		Vector operator-(Vector const& vec);
		Vector operator*(float n);
		Vector operator*(Vector const& vec);

	protected:
		float x, y, z;

	};

	
	class Vector2D : public Vector 
	{
	public:
		Vector2D(void);
		Vector2D(float x, float y);
		virtual ~Vector2D();

	private:


	};

	
	class Vector3D : public Vector
	{
	public:
		Vector3D(void);
		Vector3D(float x, float y, float z);
		virtual ~Vector3D();

	private:


	};
}
