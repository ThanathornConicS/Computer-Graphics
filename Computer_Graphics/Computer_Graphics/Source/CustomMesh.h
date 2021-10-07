#pragma once
#ifndef H_CUSTOMMESH
#define H_CUSTOMMESH

#include "Mesh.h"

class CustomMesh : public Mesh 
{
public:
	CustomMesh(std::string);

	int VertCount() override;
	Point GetVertex(int) override;
	Vector Dimension() override;
	Point Center() override;
	int FaceCount() override;
	Face GetFace(int) override;
	int EdgeCount() override;
	Edge GetEdge(int) override;

private:
	std::vector<Point> vertices;
	std::vector<Face> faces;
	std::vector<Edge> edges;
};

#endif // !H_CUSTOMMESH

