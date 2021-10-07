#include "pch.h"
#include "Affine.h"

Hcoords operator+(const Hcoords& u, const Hcoords& v)
{
	return Hcoords(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}
Hcoords operator-(const Hcoords& u, const Hcoords& v)
{
	return Hcoords(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}
Hcoords operator-(const Hcoords& v)
{
	return Hcoords(-v.x, -v.y, -v.z, -v.w);
}
Hcoords operator*(float r, const Hcoords& v)
{
	return Hcoords(v.x * r, v.y * r, v.z * r, v.w * r);
}
Hcoords operator*(const Matrix& A, const Hcoords& v)
{
	Hcoords result;

	result.x = (A[0].x * v.x) + (A[0].y * v.y) + (A[0].z * v.z) + (A[0].w * v.w);
	result.y = (A[1].x * v.x) + (A[1].y * v.y) + (A[1].z * v.z) + (A[1].w * v.w);
	result.z = (A[2].x * v.x) + (A[2].y * v.y) + (A[2].z * v.z) + (A[2].w * v.w);
	result.w = (A[3].x * v.x) + (A[3].y * v.y) + (A[3].z * v.z) + (A[3].w * v.w);

	return result;
}
Matrix operator*(const Matrix& A, const Matrix& B)
{
	Matrix result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = Hcoords((A[i].x * B[0].x) + (A[i].y * B[1].x) + (A[i].z * B[2].x) + (A[i].w * B[3].x),
			(A[i].x * B[0].y) + (A[i].y * B[1].y) + (A[i].z * B[2].y) + (A[i].w * B[3].y),
			(A[i].x * B[0].z) + (A[i].y * B[1].z) + (A[i].z * B[2].z) + (A[i].w * B[3].z),
			(A[i].x * B[0].w) + (A[i].y * B[1].w) + (A[i].z * B[2].w) + (A[i].w * B[3].w));
	}

	return result;
}

/* OPERATION */
float Dot(const Vector& u, const Vector& v)
{
	return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}
float Abs(const Vector& v)
{
	return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
Vector Cross(const Vector& u, const Vector& v)
{
	return Vector((u.y * v.z) - (v.y * u.z),
		-((u.x * v.z) - (v.x * u.z)),
		(u.x * v.y) - (v.x * u.y));
}
Matrix Perspective(float dist)
{
	Matrix persMat;

	persMat[0] = {1, 0, 0, 0};
	persMat[1] = {0, 1, 0, 0};
	persMat[2] = {0, 0, 0, 0};
	persMat[3] = {0, 0, -1.0f / dist, 1};

	return persMat;
}
Affine Rot(float t, const Vector& v)
{
	// R = (cos(theta) * I) + (((1 - cos(theta)) / (magnitude(v)^2)) * v * v^T) + ((sin(theta) / magnitude(v)) * LambdaV)
	float _cos = cos(t);
	float vMag = Abs(v);
	float _sinByMag = sin(t) / vMag;
	Affine firstTerm = Affine(Vector(_cos, 0, 0), Vector(0, _cos, 0), Vector(0, 0, _cos), Point());

	float _cosMin1 = (1.0f - _cos) / (vMag * vMag);
	Affine secondTerm = Affine(Vector(v.x * v.x * _cosMin1, v.y * v.x * _cosMin1, v.z * v.x * _cosMin1),
		Vector(v.x * v.y * _cosMin1, v.y * v.y * _cosMin1, v.z * v.y * _cosMin1),
		Vector(v.x * v.z * _cosMin1, v.y * v.z * _cosMin1, v.z * v.z * _cosMin1),
		Point());

	Affine thirdTerm = Affine(Vector(0, v.z * _sinByMag, -v.y * _sinByMag),
		Vector(-v.z * _sinByMag, 0, v.x * _sinByMag),
		Vector(v.y * _sinByMag, -v.x * _sinByMag, 0),
		Point());

	return Affine(Vector(firstTerm[0].x + secondTerm[0].x + thirdTerm[0].x, firstTerm[1].x + secondTerm[1].x + thirdTerm[1].x, firstTerm[2].x + secondTerm[2].x + thirdTerm[2].x),
		Vector(firstTerm[0].y + secondTerm[0].y + thirdTerm[0].y, firstTerm[1].y + secondTerm[1].y + thirdTerm[1].y, firstTerm[2].y + secondTerm[2].y + thirdTerm[2].y),
		Vector(firstTerm[0].z + secondTerm[0].z + thirdTerm[0].z, firstTerm[1].z + secondTerm[1].z + thirdTerm[1].z, firstTerm[2].z + secondTerm[2].z + thirdTerm[2].z),
		Point());
}
Affine Trans(const Vector& v)
{
	return Affine(Hcoords(1.0f, 0.0f, 0.0f, 0.0f),
		Hcoords(0.0f, 1.0f, 0.0f, 0.0f),
		Hcoords(0.0f, 0.0f, 1.0f, 0.0f),
		Point(v.x, v.y, v.z));
}
Affine Scale(float r)
{
	return Affine(Hcoords(r, 0.0f, 0.0f, 0.0f),
		Hcoords(0.0f, r, 0.0f, 0.0f),
		Hcoords(0.0f, 0.0f, r, 0.0f),
		Point());
}
Affine Scale(float rx, float ry, float rz)
{
	return Affine(Hcoords(rx, 0.0f, 0.0f, 0.0f),
		Hcoords(0.0f, ry, 0.0f, 0.0f),
		Hcoords(0.0f, 0.0f, rz, 0.0f),
		Point());
}
Affine Inverse(const Affine& A)
{
	float determinant = 0.0f;
	Affine cofactor;
	cofactor[0] = Hcoords((A[1][1] * A[2][2]) - (A[2][1] * A[1][2]), -((A[1][0] * A[2][2]) - (A[2][0] * A[1][2])), (A[1][0] * A[2][1]) - (A[2][0] * A[1][1]), 0);
	cofactor[1] = Hcoords(-((A[0][1] * A[2][2]) - (A[2][1] * A[0][2])), (A[0][0] * A[2][2]) - (A[2][0] * A[0][2]), -((A[0][0] * A[2][1]) - (A[2][0] * A[0][1])), 0);
	cofactor[2] = Hcoords((A[0][1] * A[1][2]) - (A[1][1] * A[0][2]), -((A[0][0] * A[1][2]) - (A[1][0] * A[0][2])), (A[0][0] * A[1][1]) - (A[1][0] * A[0][1]), 0);
	cofactor[3] = Point();

	determinant = (A[0][0] * cofactor[0][0]) + (A[0][1] * cofactor[0][1]) + (A[0][2] * cofactor[0][2]);

	if (abs(determinant - 0.0f) < 1e-5f)
	{
		L_SYSTEM_WARN("Error: This determinant has no inverse");
		return A;
	}

	Affine transposeMatrix = Affine(cofactor[0], cofactor[1], cofactor[2], Point());
	determinant = 1.0f / determinant;

	transposeMatrix[0] = determinant * transposeMatrix[0];
	transposeMatrix[1] = determinant * transposeMatrix[1];
	transposeMatrix[2] = determinant * transposeMatrix[2];

	return transposeMatrix * Trans(Vector(-A[0][3], -A[1][3], -A[2][3]));
}
