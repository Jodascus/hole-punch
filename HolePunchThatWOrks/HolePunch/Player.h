#pragma once
#include "Boxer.h"


class Player : public Boxer
{
public:
	Player();
	Player(int h);
	void TakeDamage(int);
	void Update();
	bool Punch(Boxer*) override;
	float timesWon;//Number of victorious fights
	void StartFight(time_t t);
private:
	float timePassed;//The amount of time since the fight state begins.
	time_t startTime;
};