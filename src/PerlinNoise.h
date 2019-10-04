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

	// Try to avoid octaves higher than 5, persist determines the influence of higher octaves
	void Init(int octaves, float persist);

	// Sample at x(0 - 1), y(0 - 1) with amplitude of mult with a flat bias value (0 no bias), 
	// contrast pushes values to extremes
	float SampleNoise(float x, float y, float mult, float bias, int dir, float contrast);
protected:
	std::vector<std::vector<std::vector<float>>> m_gridVals;
	int m_octaves;
	float m_persistence;

	// Sample at (x,y) with a range of (0 - octave size) in octave (oct) 
	// with octave array size of (max). Contrast push values to extremes in direction dir(-1, 0, 1)
	float SampleOctave(float x, float y, int oct, int max, float contrast, int dir);

	int RandInt(int min, int max);
	float RandFloat(float min, float max);
	float Lerp(float a, float b, float x);
	float Smooth(float t);
};