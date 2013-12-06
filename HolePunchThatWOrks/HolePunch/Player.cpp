#include "Player.h"

Player::Player() : Boxer()
{
}

Player::Player(int h) : Boxer(h)
{
}

bool Player::Punch(Boxer* b)
{
	return false;
}