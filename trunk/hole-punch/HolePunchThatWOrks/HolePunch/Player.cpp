#include "Player.h"

Player::Player() : Boxer()
{
	startTime = time(NULL);
}

Player::Player(int h) : Boxer(h)
{
}

void Player::StartFight(time_t t)
{
	startTime = t;
	timesWon++;
	health = 10;
}

void Player::Update()
{
	timePassed = difftime(time(NULL), startTime);
}

bool Player::Punch(Boxer* b)
{
	if (timePassed > (10.0 + timesWon*0.1))
	{
		b->TakeDamage(1000000000);
		return true;
	}	
	return false;
}