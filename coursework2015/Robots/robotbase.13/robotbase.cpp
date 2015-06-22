// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C"
int flag = 0;

struct robotsinf
{
	CString name;
	bool friendly;
	double lengthTo;
	double P;
	int E,L;
	bool alive;
	int i;
	int x, y;
};

struct OBJECTS
{
	
	int x;
	int y;
	double lengthTo;
};

float gain=0;






void DoStep(stepinfo *Info, step *Step)
{
	srand(time(0));


	int myNumber = Info->yourNumber;
	int myID = Info->robots[myNumber]->playerid;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	double myR = Info->field->Rmax*Info->robots[myNumber]->V / Info->field->Lmax*Info->robots[myNumber]->E / Info->field->Emax;

	int rivals = Info->field->rivals;
	robotsinf * robotsinfo;
	robotsinfo = new robotsinf[rivals - 1];
	for (int i = 0; i < rivals - 2; i++)
	{
		robotsinfo[i].x = Info->robots[i]->x;
		robotsinfo[i].y = Info->robots[i]->y;
		robotsinfo[i].E = Info->robots[i]->E;
		robotsinfo[i].L = Info->robots[i]->L;
		robotsinfo[i].name = Info->robots[i]->name;
		robotsinfo[i].lengthTo = sqrt(pow(abs(myX - Info->robots[i]->x), 2) + pow(abs(myY - Info->robots[i]->y), 2));
		robotsinfo[i].P = Info->robots[i]->P;

	}

	OBJECTS CHARGER;
	OBJECTS TECH;
	CHARGER.lengthTo = 1111111;
	TECH.lengthTo = 111111;
	for (int i = 0; i < Info->field->Ne + Info->field->Nl - 1; i++)
	{

		double buf;
		int d;
		if (Info->stepnum>0.95*Info->field->N)
			d = 2;
		else d = 1;
		if (Info->objects[i]->type == -2)
		{

			buf = sqrt(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2));
			if (CHARGER.lengthTo > buf)
			{
				int danger = 0;
				for (int j = 0; j < rivals - 2; j++)
				{
					if (abs(Info->robots[j]->alive))
					{
						double theirlength = sqrt(pow(abs(Info->robots[j]->x - Info->objects[i]->x), 2) + pow(abs(Info->robots[j]->y - Info->objects[i]->y), 2));
						if (theirlength < Info->field->Rmax)
							danger++;
					}
				}
				if (danger <= d)
				{
					CHARGER.lengthTo = buf;
					CHARGER.x = Info->objects[i]->x;
					CHARGER.y = Info->objects[i]->y;
				}
			}
		}
		if (Info->objects[i]->type == -3)
		{
			buf = sqrt(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2));
			if (TECH.lengthTo > buf)
			{
				int danger = 0;
				for (int j = 0; j < rivals - 2; j++)
				{
					if (abs(Info->robots[j]->alive))
					{
						double theirlength = sqrt(pow(abs(Info->robots[j]->x - Info->objects[i]->x), 2) + pow(abs(Info->robots[j]->y - Info->objects[i]->y), 2));
						if (theirlength < Info->field->Rmax)
							danger++;
					}
				}

				
				if (danger <= d)
				{  
					TECH.lengthTo = buf;
					TECH.x = Info->objects[i]->x;
					TECH.y = Info->objects[i]->y;
				}
			}
		}

	}







int minP = 1000;
	int minPID = -1;
	for (int i = 0; i < rivals - 2; i++)
	{
		
		if (robotsinfo[i].E <= minP && Info->robots[i]->alive && Info->robots[i]->playerid != myID && Info->robots[i]->name!="robotbase.06")
			{
				minP = robotsinfo[i].E;
				minPID = i;
			}
	}







	if (Info->robots[myNumber]->E < 0.7*Info->field->Emax || Info->stepnum>0.95*Info->field->N)
	{


		if (CHARGER.lengthTo <= ((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))))
		{
			
			if (CHARGER.y - myY && CHARGER.x - myX == 0 && Info->robots[myNumber]->E <= 0.9* Info->field->Emax)
			{
				delete[]robotsinfo;
				return;
			}
			else
			{
				DoAction(Step, ACT_TECH, 0, (1.0)*(Info->robots[myNumber]->L), (0)*(Info->robots[myNumber]->L));
				DoAction(Step, ACT_MOVE, CHARGER.x - myX, CHARGER.y - myY);
			}

		}
		else{

			int movex = abs(myX - CHARGER.x)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))) / CHARGER.lengthTo;
			int movey = abs(myY - CHARGER.y)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))) / CHARGER.lengthTo;

			DoAction(Step, ACT_TECH, 0, (0.4)*(Info->robots[myNumber]->L), (0.6)*(Info->robots[myNumber]->L));
			DoAction(Step, ACT_MOVE, movex, movey);
		}
	}
	else
	{
		if (Info->robots[myNumber]->L < 0.9*Info->field->Lmax)
		{

			if (TECH.lengthTo <= ((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))))
			{

				if (TECH.y - myY && TECH.x - myX == 0 && Info->robots[myNumber]->E <= 0.9* Info->field->Emax)
				{
					delete[]robotsinfo;
					return;
				}
				else
				{
					DoAction(Step, ACT_TECH, 0, (1.0)*(Info->robots[myNumber]->L), (0)*(Info->robots[myNumber]->L));
					DoAction(Step, ACT_MOVE, TECH.x - myX, TECH.y - myY);
				}
			}
			else{

				int movex = abs(myX - TECH.x)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))) / TECH.lengthTo;
				int movey = abs(myY - TECH.y)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))) / TECH.lengthTo;

				DoAction(Step, ACT_TECH, 0, (0.4)*(Info->robots[myNumber]->L), (0.6)*(Info->robots[myNumber]->L));
				DoAction(Step, ACT_MOVE, movex, movey);
			}
		}

		else
		{

			int targetNumber = -1, targetX, targetY, targetlengthTo;
			if (Info->robots[minPID]->playerid != myID && Info->robots[minPID]->alive)
			{
				targetNumber = minPID;
				targetX = Info->robots[targetNumber]->x;
				targetY = Info->robots[targetNumber]->y;
				targetlengthTo = sqrt(pow(abs(myX - Info->robots[targetNumber]->x), 2) + pow(abs(myY - Info->robots[targetNumber]->y), 2));
			}
			if (targetNumber == -1)
			{

				int myStep = ((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax)));
				int movex = int(myStep / 2);
				int movey = int(myStep / 2);
				DoAction(Step, ACT_MOVE, movex, movey);
				delete[]robotsinfo;
				return;

			}
			
			if (targetlengthTo <= ((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))))


			{
				DoAction(Step, ACT_MOVE, targetX - myX - 1, targetY - myY - 1);
				if (((Info->field->rndmax)*(Info->robots[myNumber]->A)*(Info->robots[myNumber]->E) / (Info->field->Emax)) > ((1 - Info->field->rndmax)*(Info->robots[targetNumber]->P)*(Info->robots[targetNumber]->E) / (Info->field->Emax)))
				{
					if ((Info->field->Rmax)*(Info->robots[myNumber]->V)*(Info->robots[myNumber]->E) / ((Info->field->Emax)*(Info->field->Lmax)))
					{
						DoAction(Step, ACT_ATTACK, targetX - myX, targetY - myY);
						gain = 0;
					}
				}
				else {
					DoAction(Step, ACT_TECH, (gain + 0.45)*(Info->robots[myNumber]->L), (0.20 - 2 * gain)*(Info->robots[myNumber]->L), (gain + 0.35)*(Info->robots[myNumber]->L));
					gain = gain + 0.05;
				}
			}
			else
			{
				DoAction(Step, ACT_TECH, (0.45)*(Info->robots[myNumber]->L), (0.20)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		

				int movex = abs(myX - targetX)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))) / targetlengthTo;
				int movey = abs(myY - targetY)*((Info->robots[myNumber]->V)*(Info->field->Vmax)*(Info->robots[myNumber]->E) / ((Info->field->Lmax)*(Info->field->Emax))) / targetlengthTo;
				DoAction(Step, ACT_MOVE, movex, movey);
			}
		}
	}
			delete[]robotsinfo;
			return;

	}
