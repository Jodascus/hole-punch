#pragma once
#include "Boxer.h"


class Player : public Boxer
{
public:
	Player();
	Player(int h);
	void TakeDamage(int);
	bool Punch(Boxer*) override;
	
};