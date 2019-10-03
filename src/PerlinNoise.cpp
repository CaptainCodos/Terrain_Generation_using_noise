#include "PerlinNoise.h"

#include <ctime>
#include <cmath>

using namespace std;
using namespace glm;

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::~PerlinNoise()
{
}

void PerlinNoise::Init(int octaves, float persist)
{
	m_octaves = octaves;
	m_persistence = persist;

	for (int i = 0; i < octaves; i++)
	{
		int maxS = (std::pow(2, i) * 8);
		m_gridVals.push_back(vector<vector<float>>(maxS, vector<float>(maxS)));
	}

	for (int i = 0; i < octaves; i++)
	{
		int maxS = (std::pow(2, i) * 8);

		for (int x = 0; x < maxS; x++)
		{
			for (int y = 0; y < maxS; y++)
			{
				m_gridVals[i][x][y] = RandFloat(0.0f, 1.0f);
			}
		}
	}
}

float PerlinNoise::SampleNoise(float x, float y, float mult, float bias, int dir, float contrast)
{
	float totalVal = 0.0f;
	float max = 0.0f;
	float amplitude = 1.0f;
	float freq = 1.0f * 8;

	for (int i = 0; i < m_octaves; i++)
	{
		int maxS = (std::pow(2, i) * 8);

		float val = SampleOctave((x * freq), (y * freq), i, freq, maxS, bias, dir) * amplitude;

		totalVal += val;

		max += amplitude;

		amplitude *= m_persistence;
		freq *= 2.0f;
	}

	float finalVal = totalVal / max;

	return pow(finalVal, contrast) * mult;
}

float PerlinNoise::SampleOctave(float x, float y, int oct, float frequency, int max, float bias, int dir)
{
	int x0, x1;
	int y0, y1;
	int Ix = (int)x;
	int Iy = (int)y;

	float fX = x - (float)(Ix);
	float fY = y - (float)(Iy);

	x0 = (Ix) % (max);
	x1 = (Ix + 1) % (max);

	y0 = (Iy) % (max);
	y1 = (Iy + 1) % (max);

	float a = m_gridVals[oct][x0][y0];
	float b = m_gridVals[oct][x1][y0];
	float c = m_gridVals[oct][x0][y1];
	float d = m_gridVals[oct][x1][y1];

	float ab = Lerp(a, b, fX);
	float cd = Lerp(c, d, fX);

	ab = Smooth(ab);
	cd = Smooth(cd);

	float abcd = Lerp(ab, cd, fY);
	abcd = Smooth(abcd);

	if (dir == 1)
	{
		abcd = pow(abcd, bias);
	}
	else if (dir == -1)
	{
		abcd = pow(abcd, 1.0f / bias);
	}

	return abcd;
}

int PerlinNoise::RandInt(int min, int max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}

float PerlinNoise::RandFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}

float PerlinNoise::Lerp(float a, float b, float x)
{
	return a + x * (b - a);
}

float PerlinNoise::Smooth(float t)
{
	return (6 * pow(t, 5)) - (15 * pow(t, 4)) + (10 * pow(t, 3));
}