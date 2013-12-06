#include "Enemy.h"

Enemy::~Enemy()
{
	delete player;
}

Enemy::Enemy() : Boxer()
{
	rng = new RandomNG();
	rng->Init();
	punchFrequency = rng->RandomInt(1,10);
	fakeoutChance = rng->RandomInt(1, 10);
	tellTime = rng->RandomInt(1, 10);
	temper = rng->RandomInt(1, 10);
}

Enemy::Enemy(int h, int punchF, float fakeoutC, float tellT, int t, Boxer* e) : Boxer(h)
{
	punchFrequency = punchF;
	fakeoutChance = fakeoutC;
	tellTime = tellT;
	temper = t;
	player = e;
}

bool Enemy::Punch(Boxer* p)
{
	bool hit = false;

	if (dodge == p->GetDodge())
	{
		p->TakeDamage(1);
		hit = true;
		numLanded++;
	}
	else
	{
		numMissed++;
	}
	stance = IDLE;
	return hit;
}

void Enemy::DetermineMood()
{
	if (numLanded > (temper * numMissed))
	{
		mood = COCKY;
	}
	else if (numMissed > (temper * numLanded))
	{
		mood = ANGRY;
	}
	else
	{
		mood = NEUTRAL;
	}
}

void Enemy::Update(float t)
{
	//First adjust dodge position to match the player's
	switch (player->GetDodge())
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

	switch (mood)//Determine punching information
	{
	case NEUTRAL://Normal punch rate and a higher chance of fakeouts. tells are normal
		if (timePassed >= punchFrequency*t)
		{
			if (fakedOut)
			{
				DeterminePunch(fakeoutChance*2, tellTime*.75);
				fakedOut = false;
			}
			else
			{
				DeterminePunch(fakeoutChance*2, tellTime);
			}
		}
		break;
	case ANGRY://Higher punch rate with a lower chance of fakeouts. tells are a bit shorter 
		if (timePassed >= punchFrequency*t*.5)
		{
			if (fakedOut)
			{
				DeterminePunch(fakeoutChance*.5, tellTime*.50);
				fakedOut = false;
			}
			else
			{
				DeterminePunch(fakeoutChance*.5, tellTime*.75);
			}
		}
		break;
	case COCKY://Lower punch rate with a normal chance of fakeouts. tells last a bit longer
		if (timePassed >= punchFrequency*t * 2)
		{
			if (fakedOut)
			{
				DeterminePunch(fakeoutChance, tellTime);
				fakedOut = false;
			}
			else
			{
				DeterminePunch(fakeoutChance, tellTime*1.25);
			}
		}
		break;
	default:
		break;
	}
}

void Enemy::DeterminePunch(float fc, float tt)
{
	float random = 0.0f;//Random number between 0 and 1
	if (random > fc)
	{
		fakedOut = true;
		stance = TELL;
		Tell(tt, true);
	}
	else
	{
		stance = TELL;
		Tell(tt, false);
	}
}

void Enemy::Tell(float tt, bool willPunch)//Waits through a countdown and finishes with a punch if willpunch is true
{
	int countdown = 1000;
	while (countdown < 1000 * tt)
	{
	}
	if (willPunch)
	{
		stance = PUNCH;
		Punch(player);
	}
	stance = IDLE;
}