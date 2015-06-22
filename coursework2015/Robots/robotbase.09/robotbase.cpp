#include "stdafx.h"
#include "robotbase.h"
#include <fstream>
#include "math.h"
using namespace std;
extern "C"

double dist(int x1, int y1, int x2, int y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}
int shag(int my, int his, double StepMax, double Dist)
{
	int shag = abs(my - his)*StepMax / Dist;
	(my - his < 0) ? shag = shag : shag = -shag;
	return shag;
}

void DoStep(stepinfo *Info, step *Step)
{
	int myNum = Info->yourNumber;
	int myX = Info->robots[myNum]->x;
	int myY = Info->robots[myNum]->y;
	int A_Num;
	double A_Dist = 100000;
	int T_Num;
	double T_Dist = 100000;
	int Aim_Num = -1;
	double Aim_Dist = 100000;
	int L = Info->robots[myNum]->L;
	int Lmax = Info->field->Lmax;
	int E = Info->robots[myNum]->E;
	int Emax = Info->field->Emax;
	int Stepnum = Info->stepnum;
	int N = Info->field->N;
	int Nl = Info->field->Nl;
	int Ne = Info->field->Ne;
	int Vmax = Info->field->Vmax;
	int Rmax = Info->field->Rmax;
	int W = Info->field->fieldWidth;
	int H = Info->field->fieldHeight;
	double StepMax = Vmax * (double(Info->robots[myNum]->V) / Lmax) * (double(E) / Emax);
	double AtakMax = Rmax * (double(Info->robots[myNum]->V) / Lmax) * (double(E) / Emax);

	for (int i = 0; i < Info->field->rivals; i++)
	{
		if ((dist(myX, myY, Info->robots[i]->x, Info->robots[i]->y) < Aim_Dist) && (Info->robots[i]->playerid != Info->robots[myNum]->playerid) && (Info->robots[i]->E > 0) && (Info->robots[i]->x != myX) && (Info->robots[i]->y != myY) &&
			(Info->robots[i]->name != "robotbase.02") && (Info->robots[i]->name != "robotbase.02.1") && (Info->robots[i]->name != "robotbase.02.2") && (Info->robots[i]->name != "robotbase.02.3") && (Info->robots[i]->name != "robotbase.02.4") && (Info->robots[i]->name != "robotbase.02.5") &&
			(Info->robots[i]->name != "robotbase.18") && (Info->robots[i]->name != "robotbase.18.1") && (Info->robots[i]->name != "robotbase.18.2") && (Info->robots[i]->name != "robotbase.18.3") && (Info->robots[i]->name != "robotbase.18.4") && (Info->robots[i]->name != "robotbase.18.5"))
		{
			Aim_Dist = dist(myX, myY, Info->robots[i]->x, Info->robots[i]->y);
			Aim_Num = i;
		}
	}

	for (int i = 0; i < Ne + Nl; i++)
	{
		if (Info->objects[i]->type == OBJ_CHARGER)
		{
			if (dist(myX, myY, Info->objects[i]->x, Info->objects[i]->y) < A_Dist)
			{
				A_Dist = dist(myX, myY, Info->objects[i]->x, Info->objects[i]->y);
				A_Num = i;
			}
		}
		if (Info->objects[i]->type == OBJ_TECH)
		{
			if (dist(myX, myY, Info->objects[i]->x, Info->objects[i]->y) < T_Dist)
			{
				T_Dist = dist(myX, myY, Info->objects[i]->x, Info->objects[i]->y);
				T_Num = i;
			}
		}
	}

	if ((E >= 0.9*Emax) && (L >= 0.9*Lmax) && (Aim_Num != -1) && (Stepnum < 0.99*N))
	{
		if (Info->robots[myNum]->A >= 0.4*L)
		{
			if (Aim_Dist < AtakMax)
			{
				DoAction(Step, ACT_TECH, 0.45*L, 0.2*L, 0.35*L);
				DoAction(Step, ACT_ATTACK, Info->robots[Aim_Num]->x - myX, Info->robots[Aim_Num]->y - myY);
			}
			else
			{
				if (Aim_Dist < StepMax)
				{
					DoAction(Step, ACT_MOVE, Info->robots[Aim_Num]->x - myX + 1, Info->robots[Aim_Num]->y - myY + 1);
				}
				else
				{
					DoAction(Step, ACT_TECH, 0, 0.4*L, 0.6*L);
					DoAction(Step, ACT_MOVE, shag(myX, Info->robots[Aim_Num]->x, StepMax, Aim_Dist), shag(myY, Info->robots[Aim_Num]->y, StepMax, Aim_Dist));
				}
			}
		}
		else
		{
			DoAction(Step, ACT_TECH, 0.45*L, 0.2*L, 0.35*L);
		}
	}
	else
	if (L < 0.9*Lmax)
	{
		if ((T_Dist > StepMax) && (StepMax > 0))
		{
			DoAction(Step, ACT_TECH, 0, 0.4*L, 0.6*L);
			DoAction(Step, ACT_MOVE, shag(myX, Info->objects[T_Num]->x, StepMax, T_Dist), shag(myY, Info->objects[T_Num]->y, StepMax, T_Dist));
		}
		else
		{
			DoAction(Step, ACT_MOVE, Info->objects[T_Num]->x - myX, Info->objects[T_Num]->y - myY);

			if ((L <= 0.89*Lmax) && (Info->objects[T_Num]->x == myX) && (Info->objects[T_Num]->y == myY))
				DoAction(Step, ACT_TECH, 0, L, 0);
			else
				DoAction(Step, ACT_TECH, 0, 0.5*L, 0.5*L);
		}
	}
	else
	if ((A_Dist > StepMax) && (StepMax > 0))
	{
		DoAction(Step, ACT_TECH, 0, 0.5*L, 0.5*L);
		DoAction(Step, ACT_MOVE, shag(myX, Info->objects[A_Num]->x, StepMax, A_Dist), shag(myY, Info->objects[A_Num]->y, StepMax, A_Dist));
	}
	else
	{
		DoAction(Step, ACT_MOVE, Info->objects[A_Num]->x - myX, Info->objects[A_Num]->y - myY);
		if ((E <= 0.89*Emax) && (Info->objects[A_Num]->x == myX) && (Info->objects[A_Num]->y == myY))
			DoAction(Step, ACT_TECH, 0, L, 0);
		else
			DoAction(Step, ACT_TECH, 0, 0.4*L, 0.6*L);
	}

	return;
}