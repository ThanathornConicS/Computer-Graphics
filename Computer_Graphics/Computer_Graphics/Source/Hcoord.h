#pragma once
#ifndef H_HCOORDS
#define H_HCOORDS

#include "pch.h"

struct Hcoords 
{
	float x, y, z, w;
	Hcoords(void);
	Hcoords(float X, float Y, float Z, float W);
	float& operator[](int i) { return *(&x + i); }
	float operator[](int i) const { return *(&x + i); }
	static bool Near(float x, float y) { return std::abs(x - y) < 1e-5f; }
};

struct Point : Hcoords 
{
	Point(void);
	Point(float X, float Y, float Z);
	Point(const Hcoords& v) : Hcoords(v) { assert(Near(w, 1)); }
};

struct Vector : Hcoords 
{
	Vector(void);
	Vector(float X, float Y, float Z);
	Vector(const Hcoords& v) : Hcoords(v) { assert(Near(w, 0)); }
	bool Normalize(void);
};

struct Matrix 
{
	Hcoords row[4];
	Matrix(void);
	Hcoords& operator[](int i) { return row[i]; }
	const Hcoords& operator[](int i) const { return row[i]; }
};

struct Affine : Matrix 
{
	Affine(void);
	Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D);
	Affine(const Matrix& M) : Matrix(M)
	{
		assert(Hcoords::Near(M[3][0], 0) && Hcoords::Near(M[3][1], 0)
			&& Hcoords::Near(M[3][2], 0) && Hcoords::Near(M[3][3], 1));
	}
};

#endif // !H_HCOORDS
