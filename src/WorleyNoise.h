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

	// Try to avoid octaves higher than 5
	void Init(int octaves, float persist);

	// Sample at x(0 - 1), y(0 - 1) with amplitude of mult with bias of pos or neg direction (0 no bias), contrast pushes values to extremes
	float SampleNoise(float x, float y, float mult, float sharpness);
protected:
	std::vector<std::vector<glm::vec2>> m_points;

	int m_octaves;
	float m_persistence;

	float SampleOctave(float x, float y, int oct, float sharpness, int max);

	int RandInt(int min, int max);
	float RandFloat(float min, float max);
};

