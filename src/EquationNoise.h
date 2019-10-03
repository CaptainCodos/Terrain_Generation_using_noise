#pragma once

#include <random>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

class EquationNoise
{
public:
	EquationNoise();
	~EquationNoise();

	// Try to avoid octaves higher than 5
	void Init(int octaves, float persist);

	// Sample at x(0 - 1), y(0 - 1) with amplitude of mult
	float SampleNoise(float x, float y, float mult);

	int RandInt(int min, int max);
	float RandFloat(float min, float max);
protected:
	std::vector<std::vector<float>> m_a;
	std::vector<std::vector<float>> m_b;
	std::vector<std::vector<float>> m_c;
	std::vector<std::vector<int>> m_s;
	std::vector<std::vector<int>> m_f;

	int m_octaves;
	float m_persistence;

	float SampleFunction(float x, float a, float b, float c, int s, int f, float mult, float freq);

	
};

