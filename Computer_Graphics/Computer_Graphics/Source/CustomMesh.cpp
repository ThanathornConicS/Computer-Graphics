#include "pch.h"
#include "CustomMesh.h"

CustomMesh::CustomMesh(std::string filepath)
{
    std::ifstream inStream;
    inStream.open(filepath.c_str(), std::ios::in);

    if (inStream.is_open()) 
    {
        for (std::string line; std::getline(inStream, line);) 
        {
            std::istringstream in(line);
            std::string type;

            in >> type;

            if (type == "v") 
            {
                float x, y, z;
                in >> x >> y >> z;

                vertices.push_back(Point(x, y, z));
            }
            else if (type == "f") 
            {
                int x, y, z;
                in >> x >> y >> z;

                //faces.push_back(Mesh::Face())
            }
        }
    }
}

int CustomMesh::VertCount()
{
    return vertices.size();
}

Point CustomMesh::GetVertex(int)
{
    return Point();
}

Vector CustomMesh::Dimension()
{
    return Vector();
}

Point CustomMesh::Center()
{
    return Point();
}

int CustomMesh::FaceCount()
{
    return faces.size();
}

Mesh::Face CustomMesh::GetFace(int i)
{
    return faces[i];
}

int CustomMesh::EdgeCount()
{
    return edges.size();
}

Mesh::Edge CustomMesh::GetEdge(int i)
{
    return edges[i];
}
