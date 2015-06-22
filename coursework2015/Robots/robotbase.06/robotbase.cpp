// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	//AfxMessageBox("DLL");
	srand(time(0));
	/*Step->action = ACT_MOVE;
	Step->dx = rand() % 3 -1;
	Step->dy = rand() % 3 -1;*/
	/*Step->numberOfActions = 1;
	Step->actions = new action[Step->numberOfActions];
	Step->actions[0].act = ACT_MOVE;
	Step->actions[0].dx = rand() % 3 - 1;
	Step->actions[0].dy = rand() % 3 - 1;*/
	//SetEvent(CommitStep);
	int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	int L = Info->robots[myNumber]->L;
	int xTech, yTech,x,y,xEner,yEner;
	int myID = Info->robots[myNumber]->playerid;
	float gain=0;
	int minP = 1000;
	int minPID = -1;
	for (int i = 0; i < Info->field->rivals - 1; i++)
	{

		if (Info->robots[i]->E <= minP && Info->robots[i]->alive && Info->robots[i]->playerid != Info->robots[myNumber]->playerid && Info->robots[i]->name!="robotbase.13")
		{
			minP = Info->robots[i]->E;
			minPID = i;
		}
	}


	for (int i = 0; i < Info->field->Ne + Info->field->Nl; i++)
	{
		if (Info->objects[i]->type == OBJ_TECH)
		{
			xTech = Info->objects[i]->x;
			yTech = Info->objects[i]->y;
		}
		if (Info->objects[i]->type == OBJ_CHARGER)
		{
			xEner = Info->objects[i]->x;
			yEner = Info->objects[i]->y;
		}
	}

	if (Info->stepnum == 1)
	{
		DoAction(Step, ACT_TECH, 0, L*0.8, L*0.2);
	}
	else
	{
		if (Info->robots[myNumber]->E < 800 && myX == xEner && myY == yEner)
			DoAction(Step, ACT_TECH, 0, L, 0);
		else
			if (Info->robots[myNumber]->E > 500)
			{
			if (Info->robots[myNumber]->L > 50 && Info->stepnum>200)
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
			if (Info->robots[myNumber]->L < 50)
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
			}
			else
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
	

	return;
}



