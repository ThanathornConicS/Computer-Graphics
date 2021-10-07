#include "pch.h"
#include "Hcoord.h"

Hcoords::Hcoords(void)
	:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

Hcoords::Hcoords(float X, float Y, float Z, float W)
    :x(X), y(Y), z(Z), w(W)
{}

Point::Point(void)
	: Hcoords(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Point::Point(float X, float Y, float Z)
    : Hcoords(X, Y, Z, 1.0f)
{}

Vector::Vector(void)
	: Hcoords(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Vector::Vector(float X, float Y, float Z)
    : Hcoords(X, Y, Z, 0.0f)
{}

bool Vector::Normalize(void)
{
	if (Near(x, 0.0f) && Near(y, 0.0f) && Near(z, 0.0f))
	{
		return false;
	}
	else
	{
		float mag = sqrt((x * x) + (y * y) + (z * z));

		x = x / mag;
		y = y / mag;
		z = z / mag;

		return true;
	}
}

Matrix::Matrix(void)
{
	for (int i = 0; i < 4; i++)
		row[i] = Hcoords();
}

Affine::Affine(void)
	: Matrix()
{
	row[3] = Point();
}
Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
	row[0].x = Lx.x;
	row[1].x = Lx.y;
	row[2].x = Lx.z;
	row[3].x = Lx.w;

	row[0].y = Ly.x;
	row[1].y = Ly.y;
	row[2].y = Ly.z;
	row[3].y = Ly.w;

	row[0].z = Lz.x;
	row[1].z = Lz.y;
	row[2].z = Lz.z;
	row[3].z = Lz.w;

	row[0].w = D.x;
	row[1].w = D.y;
	row[2].w = D.z;
	row[3].w = D.w;
}

