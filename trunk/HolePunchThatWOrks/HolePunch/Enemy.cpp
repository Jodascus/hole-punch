#include "Enemy.h"

Enemy::~Enemy()
{
	delete player;
}

Enemy::Enemy(Boxer* e) : Boxer()
{
	rng = new RandomNG();
	rng->Init();
	player = e;
	punchFrequency = rng->RandomInt(1,10);
	fakeoutChance = rng->RandomInt(1, 10);
	tellTime = rng->RandomInt(1, 10);
	temper = rng->RandomInt(1, 10);
	prevPunchT = time(NULL);
	willPunch = false;
	
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
	if (difftime(time(NULL), prevPunchT) > 1)
		stance = IDLE;

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


		timePassed = difftime(time(NULL), prevPunchT);//Update time since last punch
		switch (mood)//Determine punching information
		{
		case NEUTRAL://Normal punch rate and a higher chance of fakeouts. tells are normal
			if (timePassed >= punchFrequency*t)
			{
				if (fakedOut)
				{
					DeterminePunch(fakeoutChance * 2);
					fTellTime = tellTime*.75;
					fakedOut = false;
				}
				else
				{
					DeterminePunch(fakeoutChance * 2);
					fTellTime = tellTime;
				}
			}
			break;
		case ANGRY://Higher punch rate with a lower chance of fakeouts. tells are a bit shorter 
			if (timePassed >= punchFrequency*t*.5)
			{
				if (fakedOut)
				{
					DeterminePunch(fakeoutChance*.5);
					fTellTime = tellTime*.50;
					fakedOut = false;
				}
				else
				{
					DeterminePunch(fakeoutChance*.5);
					fTellTime = tellTime*.75;
				}
			}
			break;
		case COCKY://Lower punch rate with a normal chance of fakeouts. tells last a bit longer
			if (timePassed >= punchFrequency*t * 2)
			{
				if (fakedOut)
				{
					DeterminePunch(fakeoutChance);
					fTellTime = tellTime*2;
					fakedOut = false;
				}
				else
				{
					DeterminePunch(fakeoutChance);
					fTellTime = tellTime*1.25;
				}
			}
			break;
		default:
			break;
		}
	}
	else if (stance == TELL)
	{
		Tell();
	}
}

void Enemy::DeterminePunch(float fc)
{
	float random = rng->RandomFloat(0,1);//Random number between 0 and 1
	if (random > fc)
	{
		fakedOut = true;
		stance = TELL;
		Tell();
		willPunch = true;
		tellStartT = time(NULL);
	}
	else
	{
		stance = TELL;
		Tell();
		willPunch = false;
		tellStartT = time(NULL);
	}
}

void Enemy::Tell()//Waits through a countdown and finishes with a punch if willpunch is true
{
	
	if (difftime(time(NULL),tellStartT) > 2 * fTellTime)
	{
		if (willPunch)
		{
			stance = PUNCH;
			prevPunchT = time(NULL);
			Punch(player);
		}
		else if (!willPunch)
		{
			stance = IDLE;
			prevPunchT = time(NULL);
		}
	}
}

void Enemy::Regenerate()
{
	rng = new RandomNG();
	rng->Init();
	punchFrequency = rng->RandomInt(1, 10);
	fakeoutChance = rng->RandomInt(1, 10);
	tellTime = rng->RandomInt(1, 10);
	temper = rng->RandomInt(1, 10);
	prevPunchT = time(NULL);
	willPunch = false;
	health = 10;
}