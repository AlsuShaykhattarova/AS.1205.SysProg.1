// robotbase.cpp: определяет экспортированные функции для приложения DLL.
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
	int crite = 0.95*Info->field->Emax;
	int critl = 0.95*Info->field->Lmax;
	double maxattack = Info->field->Rmax*Info->robots[id]->V/Info->field->Lmax*e/Info->field->Emax;;
	double maxstep   = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*e/Info->field->Emax;

	//ищем союзников
	int al[3];
	int def;
	for (int i = 0; i < Info->field->rivals; i++)
	{
		if (Info->robots[i]->name == "robotbase.08" || Info->robots[i]->name == "robotbase.08.1")
			al[0] = Info->robots[i]->playerid;
		else if (Info->robots[i]->name == "robotbase.15" || Info->robots[i]->name == "robotbase.15.1")
			al[1] = Info->robots[i]->playerid;
		else if (Info->robots[i]->name == "robotbase.19" || Info->robots[i]->name == "robotbase.19.1")
			al[2] = Info->robots[i]->playerid;
		else if (Info->robots[i]->name == "robotbase.25" || Info->robots[i]->name == "robotbase.25.1")
			def = Info->robots[i]->playerid;
	}

	//определяем главного
	bool leader = true;
	for (int i = 0; i < id; i++)
	{
		if (Info->robots[i]->alive && (Info->robots[i]->playerid == al[0] || Info->robots[i]->playerid == al[1] || Info->robots[i]->playerid == al[2]))
		{
			leader = false;
			break;
		}
	}

	//если главный
	//ищем врагов, раненых и ближайшие зарядки, определяем действия
	if (leader)
	{
		bool neede = false;
		bool needl = false;

		//ищем зарядки
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

		//ищем раненых
		int closestenemy = -1;
		mindist = 9000;
		for (int i = 0; i < Info->field->rivals; i++)
		{
			if (Info->robots[i]->alive && (Info->robots[i]->playerid == al[0] || Info->robots[i]->playerid == al[1] || Info->robots[i]->playerid == al[2]))	//если свой
			{
				if (Info->robots[i]->E < crite)		//если низкая энергия
					neede = true;
				else if (Info->robots[i]->L < critl)	//если низкое техсостояние
					needl = true;
			}
			else if (Info->robots[i]->alive && Info->robots[i]->playerid != def)	//если враг
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

		//определяем действия
		ofstream file(filename);
		if (neede)
			file << 0 << endl << closestcharger;
		else if (needl)
			file << 0 << endl << closesttech;
		else
			file << 1 << endl << closestenemy;
		file.close();
	}

	//для всех
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
	case 0:		//идем на зарядку
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
			//if (e < Info->field->Emax || l < Info->field->Lmax)		//если нуждаемся в зарядке
			//{
				int v = Info->field->Vmax;
				if (v > l)
					v = l;
				DoAction(Step, ACT_TECH, 0, l-v, v);
			//}
			/*else	//если можем атаковать
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
			}*/
			break;
		}
	case 1:		//идем атаковать
		{   double maxattack = Info->field->Rmax*Info->robots[id]->V/Info->field->Lmax*e/Info->field->Emax;;
			double maxstep   = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*e/Info->field->Emax;
			
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
				if (dx != 0 && dy != 0 && Info->robots[id]->A > 0)
			{
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
			}
			break;
		}
	}

	return;
}


