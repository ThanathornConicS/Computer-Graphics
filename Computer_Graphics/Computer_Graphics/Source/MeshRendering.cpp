#include "pch.h"
#include "MeshRendering.h"

GLuint vertexBuffer;
GLuint colorBuffer;
GLuint normalBuffer;
GLuint vertexArrayID = 0;

void InitBuffer()
{
	// Prepare VAO
	if (vertexArrayID == 0)
	{
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);
		// Prepare vertexBuffer
		// This will identify our vertex buffer
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexBuffer);
		// Generate 1 buffer, put the resulting identifier in colorbuffer
		glGenBuffers(1, &colorBuffer);
		// Generate 1 buffer, put the resulting identifier in normalbuffer
		glGenBuffers(1, &normalBuffer);
	}
}

void AssignLineInfoToShader(Hcoords a, Hcoords b, Hcoords color) 
{

	GLfloat vertexBufferData[6] = { a.x, a.y, a.z, b.x, b.y, b.z };
	GLfloat colorBufferData[6] = { color.x ,color.y, color.z, color.x ,color.y, color.z };

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);
}

void AssignFaceInfoToShader(Hcoords a, Hcoords b, Hcoords c, Vector n, Hcoords color)
{

	GLfloat vertexBufferData[9] = { a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z };
	GLfloat normalBufferData[9] = { n.x, n.y, n.z, n.x, n.y, n.z, n.x, n.y, n.z };
	GLfloat colorBufferData[9] = { color.x ,color.y, color.z, color.x ,color.y, color.z, color.x ,color.y, color.z };

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalBufferData), normalBufferData, GL_STATIC_DRAW);
}

void RenderEdge(Mesh& mesh, const Affine& A, const Matrix& proj, const Vector& color)
{
	InitBuffer();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	for (int i = 0; i < mesh.EdgeCount(); i++) {
		GLuint index1 = mesh.GetEdge(i).index1;
		GLuint index2 = mesh.GetEdge(i).index2;

		Point p1 = mesh.GetVertex(index1);
		Point p2 = mesh.GetVertex(index2);

		Hcoords hp1 = A * Hcoords(p1.x, p1.y, p1.z, 1.0);
		Hcoords hp2 = A * Hcoords(p2.x, p2.y, p2.z, 1.0);

		hp1 = proj * hp1;
		hp1 = 1.0f / hp1.w * hp1;

		hp2 = proj * hp2;
		hp2 = 1.0f / hp2.w * hp2;

		AssignLineInfoToShader(hp1, hp2, color);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute 0 must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(
			1,                  // attribute 1 must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_LINES, 0, 2);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void RenderFace(Mesh& mesh, const Affine& A, const Matrix& proj, const Vector& color)
{
	InitBuffer();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (int i = 0; i < mesh.FaceCount(); i++)
	{
		GLuint index1 = mesh.GetFace(i).index1;
		GLuint index2 = mesh.GetFace(i).index2;
		GLuint index3 = mesh.GetFace(i).index3;

		Point p1 = mesh.GetVertex(index1);
		Point p2 = mesh.GetVertex(index2);
		Point p3 = mesh.GetVertex(index3);

		Hcoords hp1 = A * Hcoords(p1.x, p1.y, p1.z, 1.0);
		Hcoords hp2 = A * Hcoords(p2.x, p2.y, p2.z, 1.0);
		Hcoords hp3 = A * Hcoords(p3.x, p3.y, p3.z, 1.0);

		Vector v1 = hp2 - hp1;
		Vector v2 = hp3 - hp1;

		Vector normal = Cross(v1, v2);
		normal.Normalize();

		Point E = Point(0, 0, 3);
		Vector PE = E - hp1;

		if (Dot(normal, PE) <= 0)
			continue;

		hp1 = proj * hp1;
		hp1 = 1.0f / hp1.w * hp1;

		hp2 = proj * hp2;
		hp2 = 1.0f / hp2.w * hp2;

		hp3 = proj * hp3;
		hp3 = 1.0f / hp3.w * hp3;

		AssignFaceInfoToShader(hp1, hp2, hp3, normal, color);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer
		(
			0,				// attribute 0 must match the layout in the shader
			3,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalize
			0,				// stride
			(void*)0		// array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer
		(
			1,				// attribute 1 must match the layout in the shader
			3,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalize
			0,				// stride
			(void*)0		// array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer
		(
			2,				// attribute 1 must match the layout in the shader
			3,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalize
			0,				// stride
			(void*)0		// array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
