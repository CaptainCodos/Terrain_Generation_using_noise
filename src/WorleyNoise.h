#pragma once

#include <random>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

class WorleyNoise
{
public:
	WorleyNoise();
	~WorleyNoise();

	// Try to avoid octaves higher than 5, persist determines the influence of higher octaves
	void Init(int octaves, float persist);

	// Sample at x(0 - 1), y(0 - 1) with amplitude of mult with a flat bias value (0 no bias), 
	// sharpness pushes values to extremes to achieve sharp differences in adjacent values
	float SampleNoise(float x, float y, float mult, float sharpness);
protected:
	std::vector<std::vector<glm::vec2>> m_points;

	int m_octaves;
	float m_persistence;

	// Sample at (x,y) within a range of (0 - octave size) in octave (oct) 
	// with octave array size of (max).
	// sharpness pushes values to extremes to achieve sharp differences in adjacent values
	float SampleOctave(float x, float y, int oct, float sharpness, int max);

	int RandInt(int min, int max);
	float RandFloat(float min, float max);
};

