#pragma once

#include <random>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

// Combine multiple sin/cos functions to create wave-like noise and combine multiple octaves along x-y axes.
class EquationNoise
{
public:
	EquationNoise();
	~EquationNoise();

	// Try to avoid octaves higher than 5, persist determines the influence of higher octaves
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

	// Samples a trig function. The function used depends on values f(0, else) with the sign s(0, else)
	// taking possible forms:
	//
	// sign * a * mult * sin((freq * b * x) - (b * c))
	//						or
	// sign * a * mult * cos((freq * b * x) - (b * c))
	// with a and b as random coefficients, c an offset, x value of axis being used (x, y)
	// and freq derived from octave.
	float SampleFunction(float x, float a, float b, float c, int s, int f, float mult, float freq);
};

