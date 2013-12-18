#pragma once
#include "Graphics_Object.h"
#include "RandomNG.h"
enum DODGE { STANDING, LEFT, RIGHT };

class Boxer : Graphics_Object
{
public: 
	Boxer();
	Boxer(int h);
	void TakeDamage(int);//Take damage from being punched
	int GetHealth();//Get health value
	void SetHealth(int);//Set health value
	DODGE GetDodge();//Get current dodge state
	bool init();//initialize
	void Dodge(DODGE);//Changes the dodge state
protected:
	RandomNG* rng;//Random Number Generator
	DODGE dodge;//Current dodge state of the boxer
	int health;//How much health the boxer has
	virtual bool Punch(Boxer*){ return false; }//A punch
	

};