// robotbase.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <fstream>

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	int id = Info->yourNumber;
	int x = Info->robots[id]->x;
	int y = Info->robots[id]->y;
	int l = Info->robots[id]->L;
	int e = Info->robots[id]->E;
	string filename = "../Robots/robotbase.08/alliance.txt";
	int crite = 0.9*Info->field->Emax;
	int critl = 0.9*Info->field->Lmax;
	double maxattack = Info->field->Rmax*Info->robots[id]->V/Info->field->Lmax*e/Info->field->Emax;;
	double maxstep   = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*e/Info->field->Emax;

	//���� ���������
	int al[3];
	for (int i = 0; i < Info->field->rivals; i++)
	{
		if (Info->robots[i]->name == "robotbase.08" || Info->robots[i]->name == "robotbase.08.1")
			al[0] = Info->robots[i]->playerid;
		else if (Info->robots[i]->name == "robotbase.15" || Info->robots[i]->name == "robotbase.15.1")
			al[1] = Info->robots[i]->playerid;
		else if (Info->robots[i]->name == "robotbase.19" || Info->robots[i]->name == "robotbase.19.1")
			al[2] = Info->robots[i]->playerid;
	}

	//���������� ��������
	bool leader = true;
	for (int i = 0; i < id; i++)
	{
		if (Info->robots[i]->alive && (Info->robots[i]->playerid == al[0] || Info->robots[i]->playerid == al[1] || Info->robots[i]->playerid == al[2]))
		{
			leader = false;
			break;
		}
	}

	//���� �������
	//���� ������, ������� � ��������� �������, ���������� ��������
	if (leader)
	{
		bool neede = false;
		bool needl = false;

		//���� �������
		int closestcharger, closesttech;
		double mindist = 9000;
		for (int i = 0; i < Info->field->Ne; i++)
		{
			int cx = Info->objects[i]->x;
			int cy = Info->objects[i]->y;
			double cdist = sqrt(pow(cx - x, 2) + pow(cy - y, 2));
			if (cdist < mindist)
			{
				mindist = cdist;
				closestcharger = i;
			}
		}
		mindist = 9000;
		for (int i = Info->field->Ne; i < Info->field->Ne+Info->field->Nl; i++)
		{
			int cx = Info->objects[i]->x;
			int cy = Info->objects[i]->y;
			double cdist = sqrt(pow(cx - x, 2) + pow(cy - y, 2));
			if (cdist < mindist)
			{
				mindist = cdist;
				closesttech = i;
			}
		}

		//���� �������
		int closestenemy = -1;
		mindist = 9000;
		for (int i = 0; i < Info->field->rivals; i++)
		{
			if (Info->robots[i]->alive && (Info->robots[i]->playerid == al[0] || Info->robots[i]->playerid == al[1] || Info->robots[i]->playerid == al[2]))	//���� ����
			{
				if (Info->robots[i]->E < crite)		//���� ������ �������
					neede = true;
				else if (Info->robots[i]->L < critl)	//���� ������ ������������
					needl = true;
			}
			else if (Info->robots[i]->alive)	//���� ����
			{
				int cx = Info->robots[i]->x;
				int cy = Info->robots[i]->y;
				double cdist = sqrt(pow(cx - x, 2) + pow(cy - y, 2));
				if (cdist < mindist)
				{
					mindist = cdist;
					closestenemy = i;
				}
			}
		}

		//���������� ��������
		ofstream file(filename);
		if (neede)
			file << 0 << endl << closestcharger;
		else if (needl)
			file << 0 << endl << closesttech;
		else
			file << 1 << endl << closestenemy;
		file.close();
	}

	//��� ����
	int action, target;
	ifstream file(filename);
	file >> action >> target;
	file.close();
	if (target == -1)
	{
		action = 0;
		target = 0;
	}

	switch (action)
	{	
	case 0:		//���� �� �������
		{
			int cx = Info->objects[target]->x;
			int cy = Info->objects[target]->y;
			double cdist = sqrt(pow(cx - x, 2) + pow(cy - y, 2));
			int dx,dy;
			if (cdist > maxstep)
			{
				dx = (cx - x)*maxstep/cdist;
				dy = (cy - y)*maxstep/cdist;
			}
			else
			{
				dx = cx - x;
				dy = cy - y;
			}
			DoAction(Step, ACT_MOVE, dx, dy);
			if (e < Info->field->Emax || l < Info->field->Lmax)		//���� ��������� � �������
			{
				int v = Info->field->Vmax;
				if (v > l)
					v = l;
				DoAction(Step, ACT_TECH, 0, l-v, v);
			}
			else	//���� ����� ���������
			{
				int v = l/2;
				if (v > Info->field->Vmax)
					v = Info->field->Vmax;
				DoAction(Step, ACT_TECH, l-v-1, 1, v);
				int aim = -1;
				for (int i = 0; i < Info->field->rivals; i++)
				{
					if (Info->robots[i]->alive && Info->robots[i]->playerid != al[0] && Info->robots[i]->playerid != al[1] && Info->robots[i]->playerid != al[2])
					{
						int cx = Info->robots[i]->x;
						int cy = Info->robots[i]->y;
						double cdist = sqrt(pow(cx - x, 2) + pow(cy - y, 2));
						if (cdist <= maxattack)
						{
							dx = cx - x;
							dy = cy - y;
							DoAction(Step, ACT_ATTACK, dx, dy);
						}
					}
				}
			}
			break;
		}
	case 1:		//���� ���������
		{
			/*if (Info->history[id])
			{

				int prevx = x;
				int prevy = y;
				if (Info->history[id]->actions[ACT_MOVE])
				{
					prevx -= Info->history[id]->actions[ACT_MOVE]->dx;
					prevy -= Info->history[id]->actions[ACT_MOVE]->dy;
				}
				for (int i = 0; i < Info->field->rivals; i++)
				{
					if (Info->robots[i]->alive && Info->robots[i]->playerid != al[0] && Info->robots[i]->playerid != al[1] && Info->robots[i]->playerid != al[2])
					{
						if (Info->history[i] && Info->history[i]->actions[ACT_ATTACK])
						{
							int mdx = 0;
							int mdy = 0;
							if (Info->history[i]->actions[ACT_MOVE])
							{
								mdx = Info->history[i]->actions[ACT_MOVE]->dx;
								mdy = Info->history[i]->actions[ACT_MOVE]->dy;
							}
							int cprevx = Info->robots[i]->x - mdx;
							int cprevy = Info->robots[i]->y - mdy;
							if (cprevx + Info->history[i]->actions[ACT_ATTACK]->dx == prevx && cprevy + Info->history[i]->actions[ACT_ATTACK]->dy == prevy)		//���� ��� ��������� � ������� ����
							{
								DoAction(Step, ACT_TECH, 0, l, 0);
								return;
							}
						}
					}
				}
			}*/
			int v = l/2;
			if (v > Info->field->Vmax)
				v = Info->field->Vmax;
			DoAction(Step, ACT_TECH, l-v-1, 1, v);
			int cx = Info->robots[target]->x;
			int cy = Info->robots[target]->y;
			double cdist = sqrt(pow(cx - x, 2) + pow(cy - y, 2));
			int dx,dy;
			if (cdist > maxattack)
			{
				if (cdist > maxstep)
				{
					dx = (cx - x)*maxstep/cdist;
					dy = (cy - y)*maxstep/cdist;
				}
				else
				{
					dx = cx - x;
					dy = cy - y;
					if (cx > x)
						dx--;
					else if (cx < x)
						dx++;
					else if (cy > y)
						dy--;
					else dy++;
				}
				DoAction(Step, ACT_MOVE, dx, dy);
			}
			else
			{
				dx = cx - x;
				dy = cy - y;
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
			break;
		}
	}

	return;
}


