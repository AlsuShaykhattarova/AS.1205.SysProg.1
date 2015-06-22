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
		if (Info->robots[myNumber]->E>500)
		{
			if (myX != xTech || myY != yTech)
			{
				if (Info->robots[myNumber]->V==0)
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


