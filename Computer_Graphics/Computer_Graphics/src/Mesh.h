#pragma once
#ifndef H_MESH
#define H_MESH

#include "Affine.h"

struct Mesh 
{
	struct Face 
	{
		int index1, index2, index3;
		Face(int i, int j, int k)
			: index1(i), index2(j), index3(k) 
		{}
	};
	struct Edge 
	{
		int index1, index2;
		Edge(int i, int j)
			: index1(i), index2(j) 
		{}
	};

	virtual ~Mesh() {}
	virtual int VertCount() = 0;
	virtual Point GetVertex(int) = 0;
	virtual Vector Dimension() = 0;
	virtual Point Center() = 0;
	virtual int FaceCount() = 0;
	virtual Face GetFace(int) = 0;
	virtual int EdgeCount() = 0;
	virtual Edge GetEdge(int) = 0;
};

#endif // !H_MESH

