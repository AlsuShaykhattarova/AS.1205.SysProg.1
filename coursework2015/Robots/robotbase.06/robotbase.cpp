// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	int L = Info->robots[myNumber]->L;
	int xTech=1000, yTech=1000, x, y, xEner=1000, yEner=1000,dist=1000,distEn=1000;
	int myID = Info->robots[myNumber]->playerid;
	float gain = 0;
	int minP = 1000;
	int minPID = -1;
	for (int i = 0; i < Info->field->rivals - 1; i++)
	{

		if (Info->robots[i]->E <= minP && Info->robots[i]->alive && Info->robots[i]->playerid != Info->robots[myNumber]->playerid && Info->robots[i]->name != "robotbase.13")
		{
			minP = Info->robots[i]->E;
			minPID = i;
		}
	}


	for (int i = 0; i < Info->field->Ne + Info->field->Nl; i++)
	{
		if (Info->objects[i]->type == OBJ_TECH)
		{
			int dist1 = sqrt(pow(Info->objects[i]->x, 2) + pow(Info->objects[i]->y, 2));
			if (dist1 < dist)
			{
				dist = dist1;
				xTech = Info->objects[i]->x;
				yTech = Info->objects[i]->y;
			}
		}
		if (Info->objects[i]->type == OBJ_CHARGER)
		{
			int distEn1 = sqrt(pow(Info->objects[i]->x, 2) + pow(Info->objects[i]->y, 2));
			if (distEn1 < distEn)
			{
				distEn = distEn1;
				xEner = Info->objects[i]->x;
				yEner = Info->objects[i]->y;
			}
		}
	}



	if (Info->stepnum == 1)
	{
		DoAction(Step, ACT_TECH, 0, L, 0);
	}


	if (Info->robots[myNumber]->E < 1000 && myX == xEner && myY == yEner)
	{
		DoAction(Step, ACT_TECH, 0, L, 0);
		return;
	}
	if (Info->robots[myNumber]->L < 80 && myX == xTech && myY == yTech)
	{
		DoAction(Step, ACT_TECH, 0, L, 0);
		return;
	}


	if (Info->robots[myNumber]->E >300 && Info->stepnum<900)
	{
		if (myX != xTech || myY != yTech)
		{
			if (Info->robots[myNumber]->V == 0)
				DoAction(Step, ACT_TECH, 0, L*0.8, L*0.2);
			int maxstep = Info->robots[myNumber]->V*Info->field->Vmax / Info->field->Lmax*Info->robots[myNumber]->E / Info->field->Emax;
			if (abs(xTech - myX) > maxstep / 2)
				if (xTech > myX)
					x = maxstep / 2;
				else
					x = -maxstep / 2;
			else
				x = xTech - myX;
			if (abs(yTech - myY) > maxstep / 2)
				if (yTech > myY)
					y = maxstep / 2;
				else
					y = -maxstep / 2;
			else
				y = yTech - myY;
			DoAction(Step, ACT_MOVE, x, y);

		}
		else
			DoAction(Step, ACT_TECH, 0, L, 0);

	}


	if (Info->robots[myNumber]->E <= 800)
	{
		if (myX != xEner || myY != yEner)
		{
			if (Info->robots[myNumber]->V == 0)
				DoAction(Step, ACT_TECH, 0, L*0.8, L*0.2);
			int maxstep = Info->robots[myNumber]->V*Info->field->Vmax / Info->field->Lmax*Info->robots[myNumber]->E / Info->field->Emax;
			if (abs(xEner - myX) > maxstep / 2)
				if (xEner > myX)
					x = maxstep / 2;
				else
					x = -maxstep / 2;
			else
				x = xEner - myX;
			if (abs(yEner - myY) > maxstep / 2)
				if (yEner > myY)
					y = maxstep / 2;
				else
					y = -maxstep / 2;
			else
				y = yEner - myY;
			DoAction(Step, ACT_MOVE, x, y);

		}
	else
		DoAction(Step, ACT_TECH, 0, L, 0);
	}






	if (Info->stepnum>900)
	{
		if (myX != xEner || myY != yEner)
		{
			if (Info->robots[myNumber]->V == 0)
				DoAction(Step, ACT_TECH, 0, L*0.8, L*0.2);
			int maxstep = Info->robots[myNumber]->V*Info->field->Vmax / Info->field->Lmax*Info->robots[myNumber]->E / Info->field->Emax;
			if (abs(xEner - myX) > maxstep / 2)
				if (xEner > myX)
					x = maxstep / 2;
				else
					x = -maxstep / 2;
			else
				x = xEner - myX;
			if (abs(yEner - myY) > maxstep / 2)
				if (yEner > myY)
					y = maxstep / 2;
				else
					y = -maxstep / 2;
			else
				y = yEner - myY;
			DoAction(Step, ACT_MOVE, x, y);

		}
		else
			DoAction(Step, ACT_TECH, 0, L, 0);
	}


}




