#pragma once

#include <vector>
#include <random>

#include <glm/glm.hpp>
#include <GL/glew.h>

// Perlin noise without layered octaves
class SimplexNoise
{
public:
	static void Init(int freqPower, float t);

	static float SampleNoise(float x, float y);
private:
	static float m_cellSize;
	static int m_cells;
	static int RandInt(int min, int max);
	static float RandFloat(float min, float max);

	static std::vector<glm::vec2> m_grads;
	static std::vector<glm::vec2> m_gradients;
	static std::vector<float> m_noise;

	static float Lerp(float a, float b, float x);
	static float Smooth(float t);
};

