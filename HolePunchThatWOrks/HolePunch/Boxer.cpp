#include "Boxer.h"

Boxer::Boxer()
{
	health = 10;
	dodge = STANDING;
}

Boxer::Boxer(int h)
{
	health = h;
}

void Boxer::Dodge(DODGE d)
{
	switch (d)
	{
		case STANDING:
			dodge = STANDING;
			break;
		case LEFT:
			dodge = LEFT;
			break;
		case RIGHT:
			dodge = RIGHT;
			break;
		default:
			break;
	}
}

DODGE Boxer::GetDodge()
{
	return dodge;
}

int Boxer::GetHealth()
{
	return health;
}

void Boxer::SetHealth(int h)
{
	health = h;
}

void Boxer::TakeDamage(int d)
{
	health -= d;
	if (health < 0)
		health = 0;
}