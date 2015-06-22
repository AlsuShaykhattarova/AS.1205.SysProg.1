// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//5

#include "stdafx.h"
#include "robotbase.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include <fstream>

using namespace std;

extern "C"
void DoStep(stepinfo *Info, step *Step)
{
	int id = Info->yourNumber;
	int pid = Info->robots[id]->playerid;
	int myx = Info->robots[id]->x;
	int myy = Info->robots[id]->y;
	int L = Info->robots[id]->L;
	int E = Info->robots[id]->E;
	int Ne = Info->field->Ne;
	int Nl = Info->field->Nl;
	int Emax = Info->field->Emax;
	int V = Info->robots[id]->V;
	int Vmax = Info->field->Vmax;
	int Lmax = Info->field->Lmax;
	int Rmax = Info->field->Rmax;
	int A = Info->robots[id]->A;
	int P = Info->robots[id]->P;

	int pid2 = pid;
	for (int i = 0; i<Info->field->rivals; i++)
	{
		if (Info->robots[i]->name == "robotbase.12" || Info->robots[i]->name == "robotbase.12.1")
		{
			pid2 = Info->robots[i]->playerid;
			break;
		}
	}

	bool onE = false;
	for (int i = 0; i<Ne; i++)
	{
		if (myx == Info->objects[i]->x && myy == Info->objects[i]->y)
			onE = true;
	}

	if (E > 800 && (!onE || E > 0.95 * Emax))
	{
		DoAction(Step, ACT_TECH, 0.2 * L, 0.7 * L, 0.1 * L);
		double minDistToL = sqrt(pow(Info->objects[0]->x - myx, 2) + pow(Info->objects[0]->y - myy, 2));
		double minDistToL2 = minDistToL;
		int li1 = Ne;
		int li2 = li1;
		if (Nl > 1)
		{
			for (int i = Ne + 1; i<Nl + Ne; i++)
			{
				double curDist = sqrt(pow(Info->objects[i]->x - myx, 2) + pow(Info->objects[i]->y - myy, 2));
				if (curDist < minDistToL)
				{
					minDistToL2 = minDistToL;
					minDistToL = curDist;
					li2 = li1;
					li1 = i;
				}
			}
		}
		bool free = true;
		int robotOnL;
		for (int i = 0; i < Info->field->rivals; i++)
		{
			if (Info->robots[i]->playerid != pid && Info->robots[i]->playerid != pid2 && Info->robots[i]->x == Info->objects[li1]->x && Info->robots[i]->y == Info->objects[li1]->y)
			{
				free = false;
				robotOnL = i;
			}
		}
		double maxStep = Vmax*V / Lmax*E / Emax;
		double steps = minDistToL / maxStep;
		double maxAttack = Rmax*V / Lmax*E / Emax;
		if (free)
		{
			int dx, dy;
			if (steps < 1)
			{
				dx = Info->objects[li1]->x - Info->robots[id]->x;
				dy = Info->objects[li1]->y - Info->robots[id]->y;
			}
			else
			{
				dx = (Info->objects[li1]->x - Info->robots[id]->x) / steps;
				dy = (Info->objects[li1]->y - Info->robots[id]->y) / steps;

			}
			DoAction(Step, ACT_MOVE, dx, dy);
			for (int i = 0; i < Info->field->rivals; i++)
			{
				double curDist = sqrt(pow(Info->robots[i]->x - myx, 2) + pow(Info->robots[i]->y - myy, 2));
				if (curDist <= maxAttack && Info->robots[i]->playerid != pid && Info->robots[i]->playerid != pid2)
				{
					double myA = A * E / Emax;
					double hisP = Info->robots[i]->P * Info->robots[i]->E / Emax;
					if (myA >= hisP)
					{
						DoAction(Step, ACT_ATTACK, Info->robots[i]->x - Info->robots[id]->x, Info->robots[i]->y - Info->robots[id]->y);
						break;
					}
				}
			}
		}
		else
		{
			if (Info->robots[id]->L < Lmax)
			{
				steps = minDistToL2 / maxStep;
				int dx, dy;
				if (steps < 1)
				{
					dx = Info->objects[li2]->x - Info->robots[id]->x;
					dy = Info->objects[li2]->y - Info->robots[id]->y;
				}
				else
				{
					dx = (Info->objects[li2]->x - Info->robots[id]->x) / steps;
					dy = (Info->objects[li2]->y - Info->robots[id]->y) / steps;
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				int dx, dy;
				if (maxAttack < minDistToL)
				{
					dx = (Info->objects[li1]->x - Info->robots[id]->x) / steps;
					dy = (Info->objects[li1]->y - Info->robots[id]->y) / steps;
					DoAction(Step, ACT_MOVE, dx, dy);
				}
				else
				{
					dx = Info->objects[li1]->x - Info->robots[id]->x;
					dy = Info->objects[li1]->y - Info->robots[id]->y;
					DoAction(Step, ACT_ATTACK, dx, dy);
				}
			}
		}
	}
	else
	{
		DoAction(Step, ACT_TECH, 0.2 * L, 0.7 * L, 0.1 * L);
		double minDistToE = sqrt(pow(Info->objects[0]->x - myx, 2) + pow(Info->objects[0]->y - myy, 2));
		double minDistToE2 = minDistToE;
		int ei1 = 0;
		int ei2 = 0;
		if (Ne > 1)
		{
			for (int i = 1; i<Ne; i++)
			{
				double curDist = sqrt(pow(Info->objects[i]->x - myx, 2) + pow(Info->objects[i]->y - myy, 2));
				if (curDist < minDistToE)
				{
					minDistToE2 = minDistToE;
					minDistToE = curDist;
					ei2 = ei1;
					ei1 = i;
				}
			}
		}
		bool free = true;
		int robotOnE;
		for (int i = 0; i < Info->field->rivals; i++)
		{
			if (Info->robots[i]->playerid != pid && Info->robots[i]->playerid != pid2 && Info->robots[i]->x == Info->objects[ei1]->x && Info->robots[i]->y == Info->objects[ei1]->y)
			{
				free = false;
				robotOnE = i;
			}
		}
		double maxStep = Vmax*V / Lmax*E / Emax;
		double steps = minDistToE / maxStep;
		double maxAttack = Rmax*V / Lmax*E / Emax;
		if (free)
		{
			int dx, dy;
			if (steps < 1)
			{
				dx = Info->objects[ei1]->x - Info->robots[id]->x;
				dy = Info->objects[ei1]->y - Info->robots[id]->y;
			}
			else
			{
				dx = (Info->objects[ei1]->x - Info->robots[id]->x) / steps;
				dy = (Info->objects[ei1]->y - Info->robots[id]->y) / steps;
			}
			DoAction(Step, ACT_MOVE, dx, dy);
			for (int i = 0; i < Info->field->rivals; i++)
			{
				double curDist = sqrt(pow(Info->robots[i]->x - myx, 2) + pow(Info->robots[i]->y - myy, 2));
				if (curDist <= maxAttack && Info->robots[i]->playerid != pid && Info->robots[i]->playerid != pid2)
				{
					double myA = Info->robots[id]->A * Info->robots[id]->E / Emax;
					double hisP = Info->robots[i]->P * Info->robots[i]->E / Emax;
					if (myA >= hisP)
					{
						DoAction(Step, ACT_ATTACK, Info->robots[i]->x - Info->robots[id]->x, Info->robots[i]->y - Info->robots[id]->y);
						break;
					}
				}
			}
		}
		else
		{
			if (L < Lmax)
			{
				steps = minDistToE2 / maxStep;
				int dx, dy;
				if (steps < 1)
				{
					dx = Info->objects[ei2]->x - Info->robots[id]->x;
					dy = Info->objects[ei2]->y - Info->robots[id]->y;
				}
				else
				{
					dx = (Info->objects[ei2]->x - Info->robots[id]->x) / steps;
					dy = (Info->objects[ei2]->y - Info->robots[id]->y) / steps;
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				int dx, dy;
				if (maxAttack < minDistToE)
				{
					dx = (Info->objects[ei1]->x - Info->robots[id]->x) / steps;
					dy = (Info->objects[ei1]->y - Info->robots[id]->y) / steps;
					DoAction(Step, ACT_MOVE, dx, dy);
				}
				else
				{
					dx = Info->objects[ei1]->x - Info->robots[id]->x;
					dy = Info->objects[ei1]->y - Info->robots[id]->y;
					DoAction(Step, ACT_ATTACK, dx, dy);
				}
			}
		}
	}



	if ((P != 0.7*L) || (V != 0.3*L))
	{
		DoAction(Step, ACT_TECH, 0, 0.7*L, 0.3*L);
	}



	return;
}



