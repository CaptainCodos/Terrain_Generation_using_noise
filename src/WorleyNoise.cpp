#include "WorleyNoise.h"



using namespace std;
using namespace glm;

WorleyNoise::WorleyNoise()
{
}

WorleyNoise::~WorleyNoise()
{
}

void WorleyNoise::Init(int octaves, float persist)
{
	m_octaves = octaves;
	m_persistence = persist;

	for (int i = 0; i < octaves; i++)
	{
		int maxS = (std::pow(2, i) * 4);

		m_points.push_back(vector<vec2>(maxS * maxS));
	}

	for (int i = 0; i < octaves; i++)
	{
		int maxS = (std::pow(2, i) * 4);

		for (int x = 0; x < maxS; x++)
		{
			for (int y = 0; y < maxS; y++)
			{
				float minX = (float)x;
				float maxX = (float)x + 1;

				float minY = (float)y;
				float maxY = (float)y + 1;

				m_points[i][x + (y * maxS)] = vec2(RandFloat(minX, maxX), RandFloat(minY, maxY));
			}
		}
	}
}

float WorleyNoise::SampleNoise(float x, float y, float mult, float sharpness)
{
	float totalVal = 0.0f;
	float max = 0.0f;
	float amplitude = 1.0f;
	float freq = 1.0f * 4;

	for (int i = 0; i < m_octaves; i++)
	{
		int maxS = (std::pow(2, i) * 4);

		totalVal += SampleOctave(x * freq, y * freq, i, sharpness, maxS) * amplitude;

		max += amplitude;

		amplitude *= m_persistence;
		freq *= 2.0f;
	}

	return totalVal * mult;
}

float WorleyNoise::SampleOctave(float X, float Y, int oct, float sharpness, int max)
{
	float minDist = max * 2.0f;
	vec2 p = vec2(X, Y);

	int sX = ((int)X);
	int sY = ((int)Y);

	for (int x = sX - 1; x <= sX + 1; x++)
	{
		for (int y = sY - 1; y <= sY + 1; y++)
		{
			int iX = x % max;
			int iY = y % max;

			if (iX < 0)
				iX = max - 1;
			if (iY < 0)
				iY = max - 1;

			vec2 p2 = m_points[oct][(iY * max) + iX];
			vec2 v = p2 - p;
			float d = glm::length(v);

			if (d < minDist)
				minDist = d;
		}
	}

	return pow(std::min(1.0f, minDist), sharpness);
}

int WorleyNoise::RandInt(int min, int max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}

float WorleyNoise::RandFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}