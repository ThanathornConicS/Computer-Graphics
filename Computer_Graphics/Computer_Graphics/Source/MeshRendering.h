#pragma once
#ifndef H_MESH_RENDERING
#define H_MESH_RENDERING

#include <pch.h>

#include "Affine.h"
#include "Mesh.h"

#include <gl/glew.h>

void InitBuffer();

void AssignLineInfoToShader(Hcoords a, Hcoords b, Hcoords color);
void AssignFaceInfoToShader(Hcoords a, Hcoords b, Hcoords c, Vector n, Hcoords color);

void RenderEdge(Mesh& mesh, const Affine& A, const Matrix& proj, const Vector& color);

void RenderFace(Mesh& mesh, const Affine& A, const Matrix& proj, const Vector& color);

#endif // !H_MESH_RENDERING

