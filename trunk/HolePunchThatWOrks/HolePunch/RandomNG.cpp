#include "RandomNG.h"

RandomNG::RandomNG()
{

}

void RandomNG::Init()
{
	srand(time(NULL));
}

int RandomNG::RandomInt(int min, int max)
{
	return rand() % max + min;
}

float RandomNG::RandomFloat(float min, float max)
{
	return rand() * max + min;
}