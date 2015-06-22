// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <math.h>

using namespace std;



extern "C"

bool f = true, f1 = false;

int n = 6;

void install(stepinfo *Info, int &id1, int &id2, int &id3, int n)
{
	for (int i = 0; i < n; i++)
	{

		if (Info->robots[i]->name.Find("robotbase.24") != -1)
		{
			id1 = i;
		}
		if (Info->robots[i]->name.Find("robotbase.23") != -1)
		{
			id2 = i;
		}
		if (Info->robots[i]->name.Find("robotbase.22") != -1)
		{
			id3 = i;
		}
	}
}

void radar(stepinfo *Info, int id, int xx, int yy, int &x, int &y, int n)
{
	int min_p = Info->field->Lmax;
	int rad = Info->field->Rmax*Info->robots[id]->V / Info->field->Lmax* Info->robots[id]->E / Info->field->Emax;
	int atkx = 100000;
	int atky = 100000;
	for (int i = 0; i < n; i++)
	{
		atkx = abs(Info->robots[i]->x - xx);
		atky = abs(Info->robots[i]->y - yy);
		if (rad >= sqrt(atkx*atkx + atky*atky) && min_p>Info->robots[i]->P&&sqrt(atkx*atkx + atky*atky) != 0)
		{
			min_p = Info->robots[i]->P;
			x = Info->robots[i]->x;
			y = Info->robots[i]->y;
		}
	}
}

bool bad(stepinfo *Info, int id1, int id2, int id3)
{
	if (Info->robots[id1]->L <= 50 || Info->robots[id2]->L <= 50 || Info->robots[id3]->L <= 50)
		return true;
	else
		return false;
}

void poisk(stepinfo *Info, int &x, int &y)
{
	int rad = 10000; int xx, yy;
	for (int i = 0; i < Info->field->Nl + Info->field->Ne; i++)
	{
		if (Info->objects[i]->type == OBJ_TECH)
		{
			xx = Info->objects[i]->x;
			yy = Info->objects[i]->y;
			if (rad>(xx*xx + yy*yy))
			{
				rad = (xx*xx + yy*yy);
				x = xx;
				y = yy;
			}
		}
	}
}

void qmove(int &x, int &xx, int &y, int &yy, int ss, int &sx, int &sy)
{
	if (abs(xx - x) >= ss)
	{

		if (xx > x)
		{
			sx = -ss;
		}
		else
		{
			sx = ss;
		}
	}
	else
	{
		if (xx - x>0)
		{
			sx = -xx + x;
		}
		else
		{
			sx = x - xx;
		}
	}
	if (abs(yy - y) >= ss)
	{
		if (yy > y)
		{
			sy = -ss;
		}
		else
		{
			sy = ss;
		}
	}
	else
	{
		if (yy - y > 0)
		{
			sy = y - yy;
		}
		else
		{
			sy = y - yy;
		}
	}
}

void DoStep(stepinfo *Info, step *Step)
{
	int id = Info->yourNumber;
	int id_marat = 0;
	int id_taka = 0;
	int id_daulet = 0;
	int x, xm, xd, xt, xx;
	int y, ym, yd, yt, yy;

	xt = Info->robots[id_taka]->x;
	yt = Info->robots[id_taka]->y;
	xd = Info->robots[id_daulet]->x;
	yd = Info->robots[id_daulet]->y;
	xx = Info->robots[id]->x;
	yy = Info->robots[id]->y;
	for (int i = 0; i < Info->field->Nl + Info->field->Ne; i++)
	{
		if (Info->objects[i]->type == OBJ_CHARGER)
		{
			x = Info->objects[i]->x;
			y = Info->objects[i]->y;
			break;
		}
	}


	if (f)
	{
		id_marat = Info->robots[id]->playerid;
		install(Info, id_taka, id_daulet, id_marat, n);

		f = false;
	}
	int s, sx = 0, sy = 0;
	s = Info->field->Vmax*Info->robots[id]->V / Info->field->Lmax*Info->robots[id]->E / Info->field->Emax;

	int ss = sqrt(s);

	int atkx = 100000, atky = 100000;
	int xxx, yyy;

	if (bad(Info, id_daulet, id_marat, id_taka))
	{
		poisk(Info, xxx, yyy);
		qmove(xxx, xx, yyy, yy, ss, sx, sy);
		f1 = false;
	}
	else
	{
		if (Info->robots[id_daulet]->L == 100 && Info->robots[id_marat]->L == 100 && Info->robots[id_taka]->L == 100)
		{
			qmove(x, xx, y, yy, ss, sx, sy);
			f1 = true;
		}
		if (f1)
		{
			qmove(x, xx, y, yy, ss, sx, sy);
		}
	}


	if (xx == x&&yy == y)
	{
		DoAction(Step, ACT_TECH, 60, 30, 10);
		radar(Info, id, xx, yy, atkx, atky, n);
		if (atkx<100000 && atky<100000)
		{
			DoAction(Step, ACT_ATTACK, atkx, atky);
		}
		else
		{
			DoAction(Step, ACT_TECH, 0, 100, 0);
		}
	}
	else
	{
		DoAction(Step, ACT_MOVE, sx, sy);
		DoAction(Step, ACT_TECH, 0, 0, 100);
	}

	return;
}


