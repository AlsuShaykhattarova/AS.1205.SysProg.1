// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	int me = Info->yourNumber;
	int myx = Info->robots[me]->x;
	int myy = Info->robots[me]->y;
	double minDistanceToCharger = Info->field->fieldWidth;
	double minDistanceToTech = Info->field->fieldWidth;
	int eindex = 0;
	int tindex = Info->field->Ne;
	if (Info->field->Ne > 0)
	{
		for (int i = 0; i<Info->field->Ne + Info->field->Nl; i++)
		{
			if (Info->objects[i]->type == OBJ_CHARGER)
			{
				double curDist = sqrt(pow(Info->objects[i]->x-myx, 2) + pow(Info->objects[i]->y-myy, 2));
				if (curDist < minDistanceToCharger)
				{
					minDistanceToCharger = curDist;
					eindex = i;
				}
			}
		}
	}
	if (Info->field->Nl > 0)
	{
		for (int i = 0; i<Info->field->Ne + Info->field->Nl; i++)
		{
			if (Info->objects[i]->type == OBJ_TECH)
			{
				double curDist = sqrt(pow(Info->objects[i]->x-myx, 2) + pow(Info->objects[i]->y-myy, 2));
				if (curDist < minDistanceToTech)
				{
					minDistanceToTech = curDist;
					tindex = i;
				}
			}
		}
	}
	
	if (Info->robots[me]->E < 0.8*Info->field->Emax || Info->robots[me]->L == Info->field->Lmax)
	{
		if (minDistanceToCharger > 0)
		{
			double maxStep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[me]->E/Info->field->Emax;	
			double reqSteps = minDistanceToCharger/maxStep;	
			int destx = Info->objects[eindex]->x;
			int desty = Info->objects[eindex]->y;
			if (reqSteps > 1)
			{
				DoAction(Step,ACT_TECH,0,Info->robots[me]->L-Info->field->Vmax,Info->field->Vmax);	
				int dx = (destx - myx)/reqSteps;
				int dy = (desty - myy)/reqSteps;
				DoAction(Step,ACT_MOVE,dx,dy);
			}
			else
			{
				DoAction(Step,ACT_TECH,0,Info->robots[me]->L,0);
				DoAction(Step,ACT_MOVE,destx-myx,desty-myy);
			}
		}
		else
			DoAction(Step,ACT_TECH,0,Info->robots[me]->L,0);
	}
	else
	{
		if (minDistanceToTech > 0)
		{
			double maxStep = Info->robots[me]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[me]->E/Info->field->Emax;	
			double reqSteps = minDistanceToTech/maxStep;	
			int destx = Info->objects[tindex]->x;
			int desty = Info->objects[tindex]->y;
			if (reqSteps > 1)
			{
				DoAction(Step,ACT_TECH,0,Info->robots[me]->L-Info->field->Vmax,Info->field->Vmax);	
				int dx = (destx - myx)/reqSteps;
				int dy = (desty - myy)/reqSteps;
				DoAction(Step,ACT_MOVE,dx,dy);
			}
			else
			{
				DoAction(Step,ACT_TECH,0,Info->robots[me]->L,0);
				DoAction(Step,ACT_MOVE,destx-myx,desty-myy);
			}
		}
		else
			DoAction(Step,ACT_TECH,0,Info->robots[me]->L,0);
	}

	return;
}


