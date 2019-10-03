#include "SimplexNoise.h"

#include <ctime>

using namespace std;
using namespace glm;

float SimplexNoise::m_cellSize = 0.0f;
int SimplexNoise::m_cells = 0;
std::vector<glm::vec2> SimplexNoise::m_grads;
std::vector<glm::vec2> SimplexNoise::m_gradients;
std::vector<float> SimplexNoise::m_noise;

void SimplexNoise::Init(int freqPower, float t)
{
	srand(t);

	int size = pow(2, freqPower);
	SimplexNoise::m_cellSize = 1.0f / (float)size;
	SimplexNoise::m_cells = size;

	vec2 grads[] = {
	vec2(0.0f, pow(1.0f, 0.5)), vec2(1.0f, 1.0f),
	vec2(pow(1.0f, 0.5), 0.0f), vec2(1.0f, -1.0f),
	vec2(0.0f, -pow(1.0f, 0.5)), vec2(-1.0f, -1.0f),
	vec2(-pow(1.0f, 0.5), 0.0f), vec2(-1.0f, 1.0f)
	};

	for (int i = 0; i < 8; i++)
	{
		SimplexNoise::m_gradients.push_back(glm::normalize(grads[i]));
	}

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			vec2 v = vec2(RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f));
			v = glm::normalize(v);
			//SimplexNoise::m_grads.push_back(grads[RandInt(0, 7)]);
			SimplexNoise::m_grads.push_back(v);
			//SimplexNoise::m_noise.push_back(RandFloat(-1.0f, 1.0f));
		}
	}
}

float SimplexNoise::SampleNoise(float x, float y)
{
	int a, b, c, d;

	if (x >= 1.0f)
		x = fmod(x, 1.0f);
	if (y >= 1.0f)
		y = fmod(y, 1.0f);

	int allSize = SimplexNoise::m_cells * SimplexNoise::m_cells;

	int iX = x * SimplexNoise::m_cells;
	int iY = y * SimplexNoise::m_cells;

	//iX = iX % SimplexNoise::m_cells;
	//iY = iY % SimplexNoise::m_cells;

	a = ((iX + 1) % SimplexNoise::m_cells) + (SimplexNoise::m_cells * (iY % SimplexNoise::m_cells));
	b = ((iX + 1) % SimplexNoise::m_cells) + (SimplexNoise::m_cells * (iY % SimplexNoise::m_cells));
	c = ((iX + 1) % SimplexNoise::m_cells) + (SimplexNoise::m_cells * ((iY + 1) % SimplexNoise::m_cells));
	d = ((iX + 1) % SimplexNoise::m_cells) + (SimplexNoise::m_cells * ((iY + 1) % SimplexNoise::m_cells));

	//a = a % allSize;
	//b = b % allSize;
	//c = c % allSize;
	//d = d % allSize;

	float fX = fmod(x, SimplexNoise::m_cellSize) / SimplexNoise::m_cellSize;
	float fY = fmod(y, SimplexNoise::m_cellSize) / SimplexNoise::m_cellSize;

	float fnX = 1.0f - fX;
	float fnY = 1.0f - fY;

	vec2 Av = vec2(fX, fY); //Av = glm::normalize(Av) / pow(1.0f, 0.5f);
	vec2 Bv = vec2(fnX, fY); //Bv = glm::normalize(Bv) / pow(1.0f, 0.5f);
	vec2 Cv = vec2(fX, fnY); //Cv = glm::normalize(Cv) / pow(1.0f, 0.5f);
	vec2 Dv = vec2(fnX, fnY); //Dv = glm::normalize(Dv) / pow(1.0f, 0.5f);

	float Ad = glm::dot(SimplexNoise::m_grads[a], Av);
	float Bd = glm::dot(SimplexNoise::m_grads[b], Bv);
	float Cd = glm::dot(SimplexNoise::m_grads[c], Cv);
	float Dd = glm::dot(SimplexNoise::m_grads[d], Dv);

	//Ad = (Ad * 0.5f) + 0.5f;
	//Bd = (Bd * 0.5f) + 0.5f;
	//Cd = (Cd * 0.5f) + 0.5f;
	//Dd = (Dd * 0.5f) + 0.5f;

	float ABlerp = Lerp(Ad, Bd, fX);
	ABlerp = Smooth(ABlerp);
	float CDlerp = Lerp(Cd, Dd, fX);
	CDlerp = Smooth(CDlerp);

	float ALLlerp = Lerp(ABlerp, CDlerp, fY);

	/*if (ALLlerp >= 1.0f)
		ALLlerp = 0.99f;
	else if (ALLlerp <= -1.0f)
		ALLlerp = -0.99f;*/

	ALLlerp = Smooth(ALLlerp);

	

	return ALLlerp;
}

float SimplexNoise::Lerp(float a, float b, float x)
{
	return a + x * (b - a);
}

float SimplexNoise::Smooth(float t)
{
	bool b = t < 0.0f;
	if (b)
		t *= -1.0f;
	float ans = (6 * pow(t, 5)) - (15 * pow(t, 4)) + (10 * pow(t, 3));

	/*if (ans <= -1.0f)
	{
		ans = -1.0f;
	}*/
	//return ans;
	if (b)
		return -ans;
	else
		return ans;
}

int SimplexNoise::RandInt(int min, int max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}

float SimplexNoise::RandFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (float)(max - min)));
}

//SimplexNoise::SimplexNoise()
//{
//}
//
//
//SimplexNoise::~SimplexNoise()
//{
//}
