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

void Enemy::Update(float t, float dt)
{
	if (stance != TELL)
	{
		//First adjust dodge position to match the player's if not currently about to punch
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
	}

	timePassed = difftime(time(NULL), prevPunchT);//Update time since last punch
	switch (mood)//Determine punching information
	{
	case NEUTRAL://Normal punch rate and a higher chance of fakeouts. tells are normal
		if (timePassed >= punchFrequency*t)
		{
			if (fakedOut)
			{
				DeterminePunch(fakeoutChance*2, tellTime*.75, dt);
				fakedOut = false;
			}
			else
			{
				DeterminePunch(fakeoutChance * 2, tellTime, dt);
			}
		}
		break;
	case ANGRY://Higher punch rate with a lower chance of fakeouts. tells are a bit shorter 
		if (timePassed >= punchFrequency*t*.5)
		{
			if (fakedOut)
			{
				DeterminePunch(fakeoutChance*.5, tellTime*.50, dt);
				fakedOut = false;
			}
			else
			{
				DeterminePunch(fakeoutChance*.5, tellTime*.75, dt);
			}
		}
		break;
	case COCKY://Lower punch rate with a normal chance of fakeouts. tells last a bit longer
		if (timePassed >= punchFrequency*t* 2)
		{
			if (fakedOut)
			{
				DeterminePunch(fakeoutChance, tellTime, dt);
				fakedOut = false;
			}
			else
			{
				DeterminePunch(fakeoutChance, tellTime*1.25, dt);
			}
		}
		break;
	default:
		break;
	}
}

void Enemy::DeterminePunch(float fc, float tt, float dt)
{
	float random = rng->RandomFloat(0,1);//Random number between 0 and 1
	if (random > fc)
	{
		fakedOut = true;
		stance = TELL;
		Tell(tt, true, dt);
	}
	else
	{
		stance = TELL;
		Tell(tt, false, dt);
	}
}

void Enemy::Tell(float tt, bool willPunch, float dt)//Waits through a countdown and finishes with a punch if willpunch is true
{
	
	if (dt > 2 * tt)
	{
		if (willPunch)
		{
			stance = PUNCH;
			prevPunchT = time(NULL);
			Punch(player);
		}
		stance = IDLE;
	}
}