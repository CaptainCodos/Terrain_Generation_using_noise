#include <iostream>

#include "PolyMesh.h"
#include "ShaderProgram.h"

#include "SimplexNoise.h"
#include "PerlinNoise.h"
#include "WorleyNoise.h"
#include "EquationNoise.h"

#include <random>

PolyMesh::PolyMesh() {}

PolyMesh::PolyMesh(float width, float height, int power, TerrainType t)
{
	m_creationMode = SIMPLE;

	PerlinNoise biomes = PerlinNoise();
	biomes.Init(3, 0.3f);

	PerlinNoise perlin = PerlinNoise();
	WorleyNoise worley = WorleyNoise();
	EquationNoise eqNoise = EquationNoise();

	float water = 0.0f;
	glm::vec4 waterColor;

	std::vector<glm::vec4> biomeColors;

	// Setup noise on per-biome basis.
	switch (t)
	{
	case DesertMountain:
	{
		perlin.Init(7, 0.4f);
		worley.Init(5, 0.5f);
		eqNoise.Init(10, 0.3f);

		biomeColors.push_back(glm::vec4(0.9f, 0.75f, 0.2f, 1.0f));
		biomeColors.push_back(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		biomeColors.push_back(glm::vec4(0.35f, 0.9f, 0.15f, 1.0f));
		

		water = 1.1f;
		waterColor = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f) * 2.0f;
	}
	break;
	case Volcanic:
	{
		biomeColors.push_back(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		biomeColors.push_back(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

		perlin.Init(7, 0.4f);
		worley.Init(5, 0.6f);

		water = 3.0f;
		waterColor = glm::vec4(0.8f, 0.5f, 0.1f, 1.0f) * 4.0f;
	}
	break;
	case Highlands:
	{
		biomeColors.push_back(glm::vec4(0.2f, 0.6f, 0.1f, 1.0f));
		biomeColors.push_back(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

		perlin.Init(7, 0.4f);
		worley.Init(5, 0.5f);

		water = 1.2f;
		waterColor = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
	}
	break;
	default:
	{
		biomeColors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		perlin.Init(7, 0.4f);
	}
	break;
	}

	// Create vertices and their properties for the terrain plane.
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colors;

	int resolution = std::pow(2, power) - 3;
	float iX = (float)(width / resolution);
	float iZ = (float)(height / resolution);

	// Iterate across x-z axes to sample noise and gain y values as well as colour information.
	for (int z = 0; z < resolution; z++)
	{
		for (int x = 0; x < resolution; x++)
		{
			glm::vec2 UV = glm::vec2((float)(x / (float)resolution), (float)(z / (float)resolution));

			vector<float> samples;
			float sample = 0.0f;
			float pow = 1.0f;
			float h = 0.0f;
			glm::vec4 color;

			switch (t)
			{
			case DesertMountain:
			{
				samples.push_back(eqNoise.SampleNoise((UV.x * 2.0f), (UV.y * 2.0f), 2.0f));
				samples.push_back(worley.SampleNoise(UV.x, UV.y, 10.0f, 3.0f));
				samples.push_back(perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 2.0f, 1.0f, 0, 1.0f));

				pow = 1.0f;
				sample = perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 2.0f, 8.0f, 0, 1.0f);
			}
			break;
			case Volcanic:
			{
				samples.push_back(perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 4.0f, 1.0f, 0, 1.0f));
				samples.push_back(worley.SampleNoise(UV.x, UV.y, 12.0f, 3.0f));

				pow = 5.0f;
				sample = perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 1.0f * pow, 8.0f, 0, 1.0f);
			}
			break;
			case Highlands:
			{
				samples.push_back(perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 5.0f, 8.0f, 0, 1.0f));
				samples.push_back(worley.SampleNoise(UV.x, UV.y, 10.0f, 3.0f));

				pow = 1.3f;
				sample = perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 1.0f * pow, 8.0f, 0, 1.0f);
			}
			break;
			default:
			{
				samples.push_back(perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 4.0f, 1.0f, 0, 1.0f));

				pow = 1.0f;
				sample = perlin.SampleNoise(UV.x * 1.0f, UV.y * 1.0f, 2.0f * pow, 8.0f, 0, 1.0f);

				h = samples[0];
			}
			break;
			}
			
			int upper = std::min((int)sample + 1, (int)samples.size() - 1);
			int lower = std::min((int)sample, (int)samples.size() - 2);

			// Get interpolated terrain height and color using sample of biome perlin noise.
			float trueSample = sample / pow;
			float fX = std::fmod(trueSample, 1.0f);
			fX = (6.0f * std::pow(fX, 5.0f)) - (15.0f * std::pow(fX, 4.0f)) + (10.0f * std::pow(fX, 3.0f));
			h = (samples[lower] + fX * (samples[upper] - samples[lower]) + sample);
			color = biomeColors[lower] + fX * (biomeColors[upper] - biomeColors[lower]);

			// Flatten and color water areas.
			if (h < water)
			{
				color = waterColor;
				h = water;
			}

			glm::vec3 vert = glm::vec3(x * iX, h, z * iZ);

			verts.push_back(vert);
			UVs.push_back(UV);
			colors.push_back(color);
		}
	}

	// Vertex grid data restructured into order appropriate for rendering traingles.
	std::vector<glm::vec3> Sverts;
	std::vector<glm::vec2> SUVs;
	std::vector<glm::vec3> Snormals;
	std::vector<glm::vec4> Scolors;

	int quads = resolution - 1;

	for (int z = 0; z < quads; z++)
	{
		for (int x = 0; x < quads; x++)
		{
			int a, b, c, d;
			a = x + (z * resolution);
			b = x + (z * resolution) + 1;
			c = x + ((z + 1) * resolution);
			d = x + ((z + 1) * resolution) + 1;

			Sverts.push_back(verts[a]);
			Sverts.push_back(verts[d]);
			Sverts.push_back(verts[c]);

			Sverts.push_back(verts[a]);
			Sverts.push_back(verts[b]);
			Sverts.push_back(verts[d]);

			glm::vec3 a1 = glm::cross(verts[c] - verts[a], verts[d] - verts[a]);
			glm::vec3 a2 = glm::cross(verts[a] - verts[d], verts[c] - verts[d]);
			glm::vec3 a3 = glm::cross(verts[d] - verts[c], verts[a] - verts[c]);
			Snormals.push_back(a1);
			Snormals.push_back(a2);
			Snormals.push_back(a3);

			glm::vec3 b1 = glm::cross(verts[d] - verts[a], verts[b] - verts[a]);
			glm::vec3 b2 = glm::cross(verts[a] - verts[b], verts[d] - verts[b]);
			glm::vec3 b3 = glm::cross(verts[b] - verts[d], verts[a] - verts[d]);
			Snormals.push_back(b1);
			Snormals.push_back(b2);
			Snormals.push_back(b3);

			SUVs.push_back(UVs[a]);
			SUVs.push_back(UVs[d]);
			SUVs.push_back(UVs[c]);

			SUVs.push_back(UVs[a]);
			SUVs.push_back(UVs[b]);
			SUVs.push_back(UVs[d]);

			Scolors.push_back(colors[a]);
			Scolors.push_back(colors[d]);
			Scolors.push_back(colors[c]);

			Scolors.push_back(colors[a]);
			Scolors.push_back(colors[b]);
			Scolors.push_back(colors[d]);
		}
	}

	m_vertexNum = Sverts.size();
	verts.clear();
	UVs.clear();
	normals.clear();
	initMesh(Sverts.data(), SUVs.data(), Snormals.data(), Scolors.data());
}

void PolyMesh::initMesh(glm::vec3* vertices, glm::vec2* UVs, glm::vec3* normals, glm::vec4* colors) 
{
	std::cout << "Vertex cordinates: " << sizeof(vertices) << std::endl;
	std::cout << "Vertex cordinates: " << glm::to_string(vertices[0]) << std::endl;

	std::cout << "normals cordinates: " << sizeof(normals) << std::endl;
	std::cout << "normals cordinates: " << glm::to_string(normals[0]) << std::endl;

	// vertex array
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// vertex buffer
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * m_vertexNum, &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);
	glEnableVertexAttribArray(0);

	// texture coordinates buffer
	glGenBuffers(1, &m_tbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UVs[0]) * m_vertexNum, &UVs[0], GL_STATIC_DRAW);

	// texture coordinates array
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// normals buffer
	glGenBuffers(1, &m_nbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * m_vertexNum, &normals[0], GL_STATIC_DRAW);

	// normals array
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, (void*)0);
	glEnableVertexAttribArray(2);

	// colors buffer
	glGenBuffers(1, &m_cbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors[0]) * m_vertexNum, &colors[0], GL_STATIC_DRAW);

	// colors array
	glVertexAttribPointer(3, 4, GL_FLOAT, false, 0, (void*)0);
	glEnableVertexAttribArray(3);
}

void PolyMesh::initMesh(GLfloat* vertices, glm::vec2* UVs, glm::vec3* normals) 
{
	std::cout << "Vertex cordinates: " << sizeof(vertices) << std::endl;
	std::cout << "Vertex cordinates: " << &vertices[0] << std::endl;

	// vertex buffer
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_vertexNum, &vertices[0], GL_STATIC_DRAW);

	// vertex array
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);
	glEnableVertexAttribArray(0);
}

PolyMesh::PolyMesh(GLfloat* vertices, GLuint num, GLbyte method) 
{
	m_vertexNum = num;
	m_creationMode = SIMPLE;

	switch (method)
	{
	case 1:
		std::cout << "create geometry from list of vertex positions" << std::endl;

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * num, &vertices[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, NULL);
		glEnableVertexAttribArray(0);

		break;

	case 2:
		std::cout << "create geometry from list of vertex positions and colours" << std::endl;

		// generate vertex buffer object (allocates memory on Graphics card)
		glGenBuffers(1, &m_vbo);
		// make vbo the current (active) buffer (there can only be one at a time)
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// allocate data to buffer. We use a static buffer, meaning that the geometry data will not change
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// vertex array object, which holds information about the buffer. This is what is used to draw the genoetry, not the vertex buffer.
		glGenVertexArrays(1, &m_vao);
		// bind buffer
		glBindVertexArray(m_vao);

		// tell the graphics card what kind of data our vertex array contains
		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, NULL);
		glEnableVertexAttribArray(0);
		// colour
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
		glEnableVertexAttribArray(1);

		break;

	case 3:
		std::cout << "create geometry from list of vertex positions and UVs" << std::endl;
		// generate vertex buffer object (allocates memory on Graphics card)
		glGenBuffers(1, &m_vbo);
		// make vbo the current (active) buffer (there can only be one at a time)
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// allocate data to buffer. We use a static buffer, meaning that the geometry data will not change
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * num, &vertices[0], GL_STATIC_DRAW);

		// vertex array object, which holds information about the buffer. This is what is used to draw the genoetry, not the vertex buffer.
		glGenVertexArrays(1, &m_vao);
		// bind buffer
		glBindVertexArray(m_vao);

		// tell the graphics card what kind of data our vertex array contains
		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, NULL);
		glEnableVertexAttribArray(0);
		// UV
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
		glEnableVertexAttribArray(1);
		break;

	case 4:
		std::cout << "create geometry from list of vertex positions, colours and UVs (not implemented)" << std::endl;

		break;
	}
}

void PolyMesh::deleteMesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void PolyMesh::bindGeometry() 
{
	glBindVertexArray(getVAO());

}

void PolyMesh::draw(const Application &app)
{
	// draw geometry
	switch (getCreationMode()) 
	{
	case PolyMesh::SIMPLE:
		//std::cout << "drawing, simple mode" << std::endl;
		glDrawArrays(GL_TRIANGLES, 0, getVertexNum());
		break;
	case PolyMesh::INDEXED:
		//std::cout << "drawing, indexed mode" << std::endl;
		glDrawElements(GL_TRIANGLES, getVertexNum(), GL_UNSIGNED_INT, 0);
		break;
	}
	glBindVertexArray(0);
}