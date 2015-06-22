// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <fstream>

using namespace std;

extern "C" 

void DoStep(stepinfo *Info, step *Step)
{
	int n = Info->yourNumber;
	int pid = Info->robots[n]->playerid;
	int x = Info->robots[n]->x;
	int y = Info->robots[n]->y;
	int E = Info->robots[n]->E;
	int L = Info->robots[n]->L;
	double RA = Info->robots[n]->A * E / Info->field->Emax;

	if (Info->stepnum == 1)
	{
		ofstream file("../Robots/robotbase.03/Burdin.txt");
		file << 1;
		file.close();
	}

	if (E < 0.666 * Info->field->Emax)
	{
		ofstream file("../Robots/robotbase.03/Burdin.txt");
		file << 2;
		file.close();
	}

	if (L < 0.95 * Info->field->Lmax)
	{
		ofstream file("../Robots/robotbase.03/Burdin.txt");
		file << 4;
		file.close();
	}

	int a;
	ifstream file("../Robots/robotbase.03/Burdin.txt");
	file >> a;
	file.close();

	double attackdist = Info->field->Rmax*Info->robots[n]->V/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;
	double stepdist = Info->robots[n]->V*Info->field->Vmax/Info->field->Lmax*Info->robots[n]->E/Info->field->Emax;

	switch(a)
	{
	case 1:
		{
			DoAction(Step, ACT_TECH, 0.45*L, 0.3*L, 0.25*L);
			double dist = 1111;
			int v;
			for (int i = 0; i < Info->field->rivals; i++)
			{
				if (Info->robots[i]->alive && Info->robots[i]->playerid != pid)
				{
					int vx = Info->robots[i]->x;
					int vy = Info->robots[i]->y;
					double tmpdist = sqrt(pow(vx-x,2) + pow(vy-y,2));
					if (tmpdist < dist)
					{
						dist = tmpdist;
						v = i;
					}
				}
			}
			int vx = Info->robots[v]->x;
			int vy = Info->robots[v]->y;
			if (dist > attackdist)
			{
				int dx, dy;
				if (dist > stepdist)
				{
					dx = (vx - x)*stepdist/dist;
					dy = (vy - y)*stepdist/dist;
				}
				else
				{
					dx = vx - x;
					dy = vy - y;
					if (vx > x)
						dx-=1;
					else if (vx < x)
						dx+=1;
					else if (vy > y)
						dy-=1;
					else dy+=1;
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				double RP = Info->robots[v]->P * Info->robots[v]->E / Info->field->Emax;
				int dx, dy;
				dx = Info->robots[v]->x - Info->robots[n]->x;
				dy = Info->robots[v]->y - Info->robots[n]->y;
				if (RA>=RP)
					DoAction(Step, ACT_ATTACK, dx, dy);
				ofstream file("../Robots/robotbase.03/Burdin.txt");
				file << 2;
				file.close();
			}
			break;
		}
	case 2:
		{
			DoAction(Step, ACT_TECH, 0.45*L, 0.3*L, 0.25*L);
			int charger1 = 0;	//ближайший
			int charger2 = -1;	//с врагом
			int ex = Info->objects[charger1]->x;
			int ey = Info->objects[charger1]->y;
			double mindist = sqrt(pow(ex-x, 2) + pow(ey-y, 2));
			for (int i = 1; i<Info->field->Ne; i++)
			{
				ex = Info->objects[i]->x;
				ey = Info->objects[i]->y;
				double curdist = sqrt(pow(ex-x, 2) + pow(ey-y, 2));
				if (curdist < mindist)
				{
					mindist = curdist;
					charger1 = i;
				}
				for (int j = 0; j < Info->field->rivals; j++)
				{
					if (Info->robots[j]->alive && Info->robots[j]->playerid != pid)
					{
						int rx = Info->robots[j]->x;
						int ry = Info->robots[j]->y;
						if (rx == ex && ry == ey)
						{
							if (RA >= Info->robots[j]->P * Info->robots[j]->E / Info->field->Emax)
								charger2 = i;
						}
					}
				}
			}
			int charger;
			if (charger2 == -1 || E < 0.666 * Info->field->Emax)
			{
				charger = charger1;
				ex = Info->objects[charger]->x;
				ey = Info->objects[charger]->y;
				int dx, dy;
				double curdist = sqrt(pow(ex-x, 2) + pow(ey-y, 2));
				dx = ex - x;
				dy = ey - y;
				if (curdist > stepdist)
				{
					dx *= stepdist/curdist;
					dy *= stepdist/curdist;
				}
				else
				{
					if (E < 0.666 * Info->field->Emax)
					{
						ofstream file("../Robots/robotbase.03/Burdin.txt");
						file << 3;
						file.close();
						DoAction(Step, ACT_TECH, 0, L, 0);
					}
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				charger = charger2;
				ex = Info->objects[charger]->x;
				ey = Info->objects[charger]->y;
				int dx, dy;
				dx = ex - x;
				dy = ey - y;
				double curdist = sqrt(pow(dx, 2) + pow(dy, 2));
				if (curdist <= attackdist)
					DoAction(Step, ACT_ATTACK, dx, dy);
				else
				{
					if (curdist > stepdist)
					{
						dx *= stepdist/curdist;
						dy *= stepdist/curdist;
					}
					DoAction(Step, ACT_MOVE, dx, dy);
				}
			}
			break;
		}
	case 3:
		{
			if (E > 0.95 * Info->field->Emax)
			{
				ofstream file("../Robots/robotbase.03/Burdin.txt");
				file << 4;
				file.close();
				DoAction(Step, ACT_TECH, 0.45*L, 0.3*L, 0.25*L);
			}
			else
				DoAction(Step, ACT_TECH, 0, L, 0);
			break;
		}
	case 4:
		{
			DoAction(Step, ACT_TECH, 0.3*L, 0.4*L, 0.3*L);
			int lid = Info->field->Ne;
			int lx = Info->objects[lid]->x;
			int ly = Info->objects[lid]->y;
			double mindist = sqrt(pow(lx-x, 2) + pow(ly-y, 2));
			for (int i = Info->field->Ne+1; i<Info->field->Ne+Info->field->Nl; i++)
			{
				lx = Info->objects[i]->x;
				ly = Info->objects[i]->y;
				double curdist = sqrt(pow(lx-x, 2) + pow(ly-y, 2));
				if (curdist < mindist)
				{
					mindist = curdist;
					lid = i;
				}
			}
			lx = Info->objects[lid]->x;
			ly = Info->objects[lid]->y;
			if (stepdist < mindist)
			{
				int dx, dy;
				dx = (lx - x)*stepdist/mindist;
				dy = (ly - y)*stepdist/mindist;
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				int dx, dy;
				dx = lx - x;
				dy = ly - y;
				DoAction(Step, ACT_MOVE, dx, dy);
				if (L > 0.95 * Info->field->Lmax)
				{
					ofstream file("../Robots/robotbase.03/Burdin.txt");
					file << 1;
					file.close();
					DoAction(Step, ACT_TECH, 0.45*L, 0.3*L, 0.25*L);
				}
				else
					DoAction(Step, ACT_TECH, 0, L, 0);
			}

			bool found = false;
			int v;
			for (int i = 0; i < Info->field->rivals; i++)
			{
				if (Info->robots[i]->alive && Info->robots[i]->playerid != pid)
				{
					int vx = Info->robots[i]->x;
					int vy = Info->robots[i]->y;
					double robodist = sqrt(pow(vx-x,2) + pow(vy-y,2));
					if (robodist < attackdist)
					{
						double RP = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
						if (RP < RA)
						{
							v = i;
							found = true;
							break;
						}
					}
				}
			}
			if (found)
			{
				int vx = Info->robots[v]->x;
				int vy = Info->robots[v]->y;
				int dx = vx - x;
				int dy = vy - y;
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
			break;
		}
	}
	return;
}


