#include "EquationNoise.h"

#include "glm/ext.hpp"

using namespace std;
using namespace glm;

EquationNoise::EquationNoise()
{
}

EquationNoise::~EquationNoise()
{
}

void EquationNoise::Init(int octaves, float persist)
{
	m_octaves = octaves;
	m_persistence = persist;

	for (int i = 0; i < 2; i++)
	{
		m_a.push_back(vector<float>(octaves));
		m_b.push_back(vector<float>(octaves));
		m_c.push_back(vector<float>(octaves));

		m_s.push_back(vector<int>(octaves));
		m_f.push_back(vector<int>(octaves));
	}

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < octaves; k++)
		{
			m_a[i][k] = RandFloat(0.3f, 1.0f);
			m_b[i][k] = RandFloat(2.0f, 3.0f);
			m_c[i][k] = RandFloat(-two_pi<float>(), two_pi<float>());

			m_s[i][k] = RandInt(0, 2);
			m_f[i][k] = RandInt(0, 2);
		}
	}
}

float EquationNoise::SampleNoise(float x, float y, float mult)
{
	float X = x * two_pi<float>();
	float Y = y * two_pi<float>();

	float totalVal = 0.0f;
	float max = 0.0f;
	float amplitude = 1.0f;
	float freq = 1.0f;

	for (int i = 0; i < m_octaves; i++)
	{
		float valX = SampleFunction(X, m_a[0][i], m_b[0][i], m_c[0][i], m_s[0][i], m_f[0][i], amplitude, freq);
		float valY = SampleFunction(Y, m_a[1][i], m_b[1][i], m_c[1][i], m_s[1][i], m_f[1][i], amplitude, freq);

		float posX = valX + (m_a[0][i] * amplitude);
		float posY = valY + (m_a[1][i] * amplitude);
		totalVal += (posX + posY) / 2.0f;
		//totalVal += (valX + valY) / 2.0f;

		amplitude *= m_persistence;
		freq *= RandFloat(1.97f, 2.03f);
		freq += RandFloat(0.0f, 0.05f);
		//freq *= 4.0f;
	}

	return totalVal * mult;
}

float EquationNoise::SampleFunction(float x, float a, float b, float c, int s, int f, float mult, float freq)
{
	int sign;
	if (s == 0)
		sign = -1;
	else
		sign = 1;

	if (f == 0)
		return sign * a * mult * std::sin((freq * b * x) - (b * c));
	else
		return sign * a * mult * std::cos((freq * b * x) - (b * c));
}

int EquationNoise::RandInt(int min, int max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}

float EquationNoise::RandFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}