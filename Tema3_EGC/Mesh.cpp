#include "Mesh.hpp"

#define RESERVE_VALUE 1000

glg::Mesh::Mesh(const GLchar* inFile, GLboolean wireframe)
{
	std::ifstream objFile(inFile);
	if (!objFile.good())
		throw std::runtime_error("Cannot open OBJ file! The file does not exists or the application does not have access to it!");
	std::vector<glm::vec3> v, vn;
	std::vector<glm::vec2> vt;
	std::vector<VertexFormat> vertices; 
	vertices.reserve(RESERVE_VALUE);
	std::vector<GLuint> indices; 
	indices.reserve(RESERVE_VALUE);
	std::unordered_map<Triplet, GLuint> vindex;
	std::string line;
	// push the 0.0f values at index 0; needed for the parsing part
	v.reserve(RESERVE_VALUE); v.push_back(glm::vec3(0.0f));
	vt.reserve(RESERVE_VALUE); vt.push_back(glm::vec2(0.0f));
	vn.reserve(RESERVE_VALUE); vn.push_back(glm::vec3(0.0f));
	
	while (!objFile.eof())
	{
		std::getline(objFile, line);
		GLfloat x, y, z;

		if (line[0] == 'v')
		{
			if (line[1] == 'n')
			{
				if (sscanf(line.c_str(), "vn%f%f%f", &x, &y, &z) == 3)
					vn.push_back(glm::vec3(x, y, z));
				else throw std::runtime_error("Vertex normal format unsupported!");
			}
			else if (line[1] == 't')
			{
				if (sscanf(line.c_str(), "vt%f%f", &x, &y) == 2)
					vt.push_back(glm::vec2(x, y));
				else throw std::runtime_error("UV coordinate format unsupported!");
			}
			else
			{
				if (sscanf(line.c_str(), "v%f%f%f", &x, &y, &z) == 3)
					v.push_back(glm::vec3(x, y, z));
				else throw std::runtime_error("Vertex position format unsupported!");
			}
		}
		else if (line[0] == 'f')
		{
			// A face is composed of triangles
			// Note that we don't need to convert to 0-based indexing since we pushed those
			// zeros at the begining of this function
			// VertexFormat stores the attributes in the following order: v/vn/vt
			
			Triplet t(0);
			std::istringstream ss(&line[1]);
			std::string tok; 
			GLuint firstVertexOfFace = -1, previousVertex = -1;
			for (GLint k = 0; (ss >> tok); ++k)
			{
				if (sscanf(tok.c_str(), "%d/%d/%d", &t.x, &t.y, &t.z) == 3);
				else if (sscanf(tok.c_str(), "%d//%d", &t.x, &t.z) == 2);
				else if (sscanf(tok.c_str(), "%d/%d", &t.x, &t.y) == 2);
				else if ((sscanf(tok.c_str(), "%d", &t.x) == 1));
				else throw std::runtime_error("Face format unsupported!");

				// Correction for negative indices
				if (t.x < 0) t.x = v.size() + t.x;
				if (t.y < 0) t.y = vt.size() + t.y;
				if (t.z < 0) t.z = vn.size() + t.z;

				if (vindex.find(t) == vindex.end())
				{
					vindex[t] = vertices.size();
					vertices.push_back(VertexFormat(v[t.x].x, v[t.x].y, v[t.x].z,
						vn[t.z].x, vn[t.z].y, vn[t.z].z,
						vt[t.y].x, vt[t.y].y));
				}

				if (k < 3)
				{
					if (k == 1) firstVertexOfFace = vindex[t];
					indices.push_back(vindex[t]);
				}
				else
				{
					indices.push_back(firstVertexOfFace);
					indices.push_back(previousVertex);
					indices.push_back(vindex[t]);
				}

				previousVertex = vindex[t];
			}
		}
		else if (line[0] == 'o')
			throw std::runtime_error("Cannot read OBJ with multiple objects!");
	}
	objFile.close();

	if (v.size() == 1)
		throw std::runtime_error("OBJ file has no vertices!");

	// Create buffers to hold the mesh data
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// Configure the vertex attribute pointers
	// Send vertex positions to attrib pointer 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Send vertex normals to attrib pointer 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	// Send vertex uv texcoords to attrib pointer 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);

	// Clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	count = indices.size();
	this->wireframe = wireframe;
}

void glg::Mesh::draw()
{
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (GLvoid*)0);
}
