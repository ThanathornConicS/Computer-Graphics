#pragma once
#ifndef H_AFFINE
#define H_AFFINE

#include "pch.h"
#include "Source/Hcoord.h"

Hcoords operator+(const Hcoords& u, const Hcoords& v);
Hcoords operator-(const Hcoords& u, const Hcoords& v);
Hcoords operator-(const Hcoords& v);
Hcoords operator*(float r, const Hcoords& v);
Hcoords operator*(const Matrix& A, const Hcoords& v);
Matrix operator*(const Matrix& A, const Matrix& B);
float Dot(const Vector& u, const Vector& v);
float Abs(const Vector& v);
Vector Cross(const Vector& u, const Vector& v);
Matrix Perspective(float dist);
Affine Rot(float t, const Vector& v);
Affine Trans(const Vector& v);
Affine Scale(float r);
Affine Scale(float rx, float ry, float rz);
Affine Inverse(const Affine& A);

#endif // !H_AFFINE