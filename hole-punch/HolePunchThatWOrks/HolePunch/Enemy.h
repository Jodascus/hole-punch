#pragma once
#include "Boxer.h"
class Enemy : public Boxer
{
public:
	~Enemy();
	Enemy(Boxer* e);
	Enemy(int h, int punchF, float fakeoutC, float tellT, int t, Boxer* e);
	void Update(float, float);
	void Init();
	void Regenerate();

	enum MOOD {ANGRY, COCKY, NEUTRAL};//used to simulate a fighter getting mad or dropping their guard
	enum STANCE {IDLE, TELL, PUNCH};//used to determine which model needs to be displayed
private:
	MOOD mood;//Enemy's current mood
	STANCE stance;//Enemy's current stance
	float timePassed;//time elapsed since last punch was thrown
	time_t prevPunchT;//time value for last punch
	time_t tellStartT;//starting time for the tell
	bool willPunch;//will the boxer punch
	float fTellTime;//Final tell time after calculations

	int punchFrequency;//determines how often the AI will possibly throw a punch
	float fakeoutChance;//value between 0 and .25 that determines if the enemy performs a fakeout
	bool fakedOut;//if true the punch after the fakeout gets a shorter tell
	float tellTime;//how long the fighter's tell lasts before they punch
	int temper;//used for figuring out how easily the enemy enters the angry state
	int numMissed;//Number of enemy punches dodged by the player
	int numLanded;//Number of enemy punches landed on the player
	Boxer* player;//The player
	
	void DetermineMood();//Figures out if the AI should be getting angry, cocky, or neutral
	void DeterminePunch(float fc);//Figures out if the AI will throw a punch or fakeout the player
	void Tell();//Times out the tell pose which warns the player to dodge before they get punched
	bool Punch(Boxer*) override;//returns whether or not the player was punched successfully and damages the player
};