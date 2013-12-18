#include <stdlib.h>
#include <time.h> 

class RandomNG
{
public:
	RandomNG();
	void Init();
	int RandomInt(int min, int max);
	float RandomFloat(float min, float max);
};