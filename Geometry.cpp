#include "Geometry.h"


Geometry::Geometry()
{
}

Geometry::~Geometry()
{
}

void Geometry::init()
{
	initPrims();
	createFaces();
	createNormals();
	initInterleaved();

	//Set up the vertex attribute object (VAO)
	//We make an int to associate our VAO with.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Generate an empty buffer for the points
	glGenBuffers(1, &points_vbo);
	//Set the above as our current buffer via "binding"
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	//Copy points into the currently bound buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(interleaved.at(0)) * static_cast<int>(interleaved.size()), interleaved.data(), GL_STATIC_DRAW);

	//Generate the indices and send them to the graphics card
	glGenBuffers(1, &index_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(prim_indices.at(0)) * static_cast<int>(prim_indices.size()), prim_indices.data(), GL_STATIC_DRAW);

	//Enable the first attribute, 0, which is our Points buffer
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	//0 means to define the layout for attrib #0, 3 means that we're using vec3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(GLfloat), BUFFER_OFFSET(0));
	//Enable Normals attribute, 2
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(GLfloat), BUFFER_OFFSET(12));
}

void Geometry::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void Geometry::createFaces()
{
	for (int i = 0; i < indices.size(); i++)
	{
		faces.push_back(new Face(verts.at(indices.at(i).first), verts.at(indices.at(i).second), verts.at(indices.at(i).third)));
	}
}

void Geometry::createNormals()
{
	int normalCount = 0;
	//Get a normal for each vertex
	for (int i = 0; i < verts.size(); i++)
	{
		GLfloat vertexNormal = 0.0f;
		glm::vec3 normals;
		
		//Find the connected faces
		for (int j = 0; j < indices.size(); j++)
		{
			if (indices.at(j).first == i || indices.at(j).second == i || indices.at(j).third == i)
			{
				//Add the face's normal to our collection of normals
				normals += faces.at(j)->norm;
				normalCount++;
			}
		}
		//Create our vetex normal
		glm::vec3 normalizedNormals = normals;
		if (normals != glm::vec3(0.0f, 0.0f, 0.0f))
			normalizedNormals = glm::normalize(normals);

		//Store our vertex normal.
		norms.push_back(normalizedNormals);
	}
}

void Geometry::initPrims()
{
	for (int i = 0; i < indices.size(); i++)
	{
		prim_indices.push_back(indices.at(i).first);
		prim_indices.push_back(indices.at(i).second);
		prim_indices.push_back(indices.at(i).third);
	}
}

void Geometry::initInterleaved()
{
	for (int i = 0; i < verts.size(); i++)
	{
		interleaved.push_back(verts.at(i).x); 
		interleaved.push_back(verts.at(i).y); 
		interleaved.push_back(verts.at(i).z); 
		interleaved.push_back(norms.at(i).x);
		interleaved.push_back(norms.at(i).y);
		interleaved.push_back(norms.at(i).z);
	}
}