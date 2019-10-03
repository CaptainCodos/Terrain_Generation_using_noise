#pragma once

#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <GL/glew.h>

class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	// Try to avoid octaves higher than 5
	void Init(int octaves, float persist);

	// Sample at x(0 - 1), y(0 - 1) with amplitude of mult with bias of pos or neg direction (0 no bias), contrast pushes values to extremes
	float SampleNoise(float x, float y, float mult, float bias, int dir, float contrast);
protected:
	std::vector<std::vector<std::vector<float>>> m_gridVals;
	int m_octaves;
	float m_persistence;

	float SampleOctave(float x, float y, int oct, float freqency, int max, float bias, int dir);
	//float DotProd(int gX, int gY, int oct, float x, float y);

	int RandInt(int min, int max);
	float RandFloat(float min, float max);

	float Lerp(float a, float b, float x);
	float Smooth(float t);
};