// robotbase.cpp: определ€ет экспортированные функции дл€ приложени€ DLL.
//

#include "stdafx.h"
#include "robotbase.h"
#include <fstream>

using namespace std;

extern "C" 
	void DoStep(stepinfo *Info, step *Step)
{
	int id = Info->yourNumber;
	int pid = Info->robots[id]->playerid;
	int pid2 = pid;
	for (int i = 0; i<Info->field->rivals; i++)
	{
		if (Info->robots[i]->name == "robotbase.05" || Info->robots[i]->name == "robotbase.05.1")
		{
			pid2 = Info->robots[i]->playerid;
			break;
		}
	}

	if (Info->stepnum == 1)
	{
		ofstream File("../Robots/robotbase.12/molokanova");
		File << 1;
		File.close();
	}

	int myx = Info->robots[id]->x;
	int myy = Info->robots[id]->y;

	if (Info->robots[id]->E > 0.9 * Info->field->Emax)	//если энергии больше 900
	{
		for (int i = 0; i<Info->field->Ne; i++)	//провер€ем, не зар€жаемс€ ли мы
		{
			if (myx == Info->objects[i]->x && myy == Info->objects[i]->y && Info->robots[id]->E < 0.99 * Info->field->Emax)	//если стоим на зар€дке и энергии меньше 990
			{
				DoAction(Step, ACT_TECH, 0,0.8*Info->robots[id]->L,0.2*Info->robots[id]->L);	//распредел€ем параметры
				return;	//и больше ничего не делаем
			}
		}

		if (Info->robots[id]->L == Info->field->Lmax)	//если мы здоровы
		{
			DoAction(Step, ACT_TECH, 0.2*Info->robots[id]->L,0.7*Info->robots[id]->L,0.1*Info->robots[id]->L);
			double Lint = 0;
			int Lnum = -1;
			for (int i = Info->field->Ne; i<Info->field->Nl+Info->field->Ne; i++)	//ищем зан€тую техточку
			{
				int lx = Info->objects[i]->x;
				int ly = Info->objects[i]->y;
				for (int j = 0; j<Info->field->rivals; j++)
				{
					if (Info->robots[j]->x == lx && Info->robots[j]->y == ly && Info->robots[j]->playerid!=pid && Info->robots[j]->playerid!=pid2)	//если точка зан€та
					{
						double cint = sqrt(pow(lx-myx, 2) + pow(ly-myy, 2));
						if (cint < Lint || Lint == 0)
						{
							Lint = cint;
							Lnum = i;
						}
					}
				}
			}
			if (Lint > 0)	//если нашли
			{
				double step = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;
				double attack = Info->field->Rmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;
				if (Lint <= attack)	//если дот€гиваемс€ до точки
					DoAction(Step, ACT_ATTACK,  Info->objects[Lnum]->x - myx,Info->objects[Lnum]->y - myy);
				else
				{
					int dx = (Info->objects[Lnum]->x - myx)*step/Lint;
					int dy = (Info->objects[Lnum]->y - myy)*step/Lint;
					DoAction(Step, ACT_MOVE, dx, dy);
				}
			}
			else
			{
				ifstream File("../Robots/robotbase.12/molokanova");
				int dir;
				File >> dir;	//читаем, в каком направлении шли
				File.close();

				switch (dir)
				{
				case 1:
					{
						Lint = -1;
						for (int i = Info->field->Ne; i<Info->field->Nl+Info->field->Ne; i++)
						{
							int lx = Info->objects[i]->x;
							int ly = Info->objects[i]->y;
							double cint = sqrt(pow(lx-myx, 2) + pow(ly-myy, 2));
							if (cint < Lint || Lint == -1)
							{
								Lint = cint;
								Lnum = i;
							}
						}
						if (Lint)
						{
							double step = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;
							int dx;
							int dy;
							if (Lint > step)
							{
								dx = (Info->objects[Lnum]->x - myx)*step;///Lint;
								dy = (Info->objects[Lnum]->y - myy)*step;///Lint;
								dx /= Lint;
								dy /= Lint;
							}
							else
							{
								dx = Info->objects[Lnum]->x - myx;
								dy = Info->objects[Lnum]->y - myy;
							}
							DoAction(Step, ACT_MOVE, dx, dy);
						}
						else
						{
							ofstream File("../Robots/robotbase.12/molokanova");
							File << 2;
							File.close();
						}
						double attack = Info->field->Rmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;	//бьем всех, до кого дот€гиваемс€
						for (int i = 0; i<Info->field->rivals; i++)
						{
							double cint = sqrt(pow(Info->robots[i]->x-myx, 2) + pow(Info->robots[i]->y-myy, 2));
							if (cint <= attack && Info->robots[i]->playerid!=pid && Info->robots[i]->playerid!=pid2)
							{
								double A = Info->robots[id]->A * Info->robots[id]->E / Info->field->Emax;
								double P = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
								if (A >= P)
									DoAction(Step, ACT_ATTACK, Info->robots[i]->x - myx,Info->robots[i]->y - myy);
							}
						}
						break;
					}
				case 2:
					{
						int Eint = -1;
						int Enum;
						for (int i = 0; i<Info->field->Ne; i++)
						{
							int ex = Info->objects[i]->x;
							int ey = Info->objects[i]->y;
							double cint = sqrt(pow(ex-myx, 2) + pow(ey-myy, 2));
							if (cint < Eint || Eint == -1)
							{
								Eint = cint;
								Enum = i;
							}
						}
						if (Eint)
						{
							double step = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;
							int dx;
							int dy;
							if (Eint > step)
							{
								dx = (Info->objects[Enum]->x - myx)*step;///Eint;
								dy = (Info->objects[Enum]->y - myy)*step;///Eint;
								dx /= Eint;
								dy /= Eint;
							}
							else
							{
								dx = Info->objects[Enum]->x - myx;
								dy = Info->objects[Enum]->y - myy;
							}
							DoAction(Step, ACT_MOVE, dx, dy);
						}
						else
						{
							ofstream File("../Robots/robotbase.12/molokanova");
							File << 1;
							File.close();
						}
						double attack = Info->field->Rmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;	//бьем всех, до кого дот€гиваемс€
						for (int i = 0; i<Info->field->rivals; i++)
						{
							double cint = sqrt(pow(Info->robots[i]->x-myx, 2) + pow(Info->robots[i]->y-myy, 2));
							if (cint <= attack && Info->robots[i]->playerid!=pid && Info->robots[i]->playerid!=pid2)
							{
								double A = Info->robots[id]->A * Info->robots[id]->E / Info->field->Emax;
								double P = Info->robots[i]->P * Info->robots[i]->E / Info->field->Emax;
								if (A >= P)
									DoAction(Step, ACT_ATTACK, Info->robots[i]->x - myx, Info->robots[i]->y - myy);
							}
						}
						break;
					}
				}
			}
		}
		else  //если надо подлечитьс€
		{
			DoAction(Step, ACT_TECH, 0,0.7*Info->robots[id]->L,0.3*Info->robots[id]->L);
			double Lint = -1;
			int Lnum = -1;
			for (int i = Info->field->Ne; i<Info->field->Nl+Info->field->Ne; i++)
			{
				int lx = Info->objects[i]->x;
				int ly = Info->objects[i]->y;
				double cint = sqrt(pow(lx-myx, 2) + pow(ly-myy, 2));
				if (cint < Lint || Lint == -1)
				{
					Lint = cint;
					Lnum = i;
				}
			}
			double step = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;
			int dx;
			int dy;
			if (Lint > step)
			{
				dx = (Info->objects[Lnum]->x - myx)*step/Lint;
				dy = (Info->objects[Lnum]->y - myy)*step/Lint;
			}
			else
			{
				dx = Info->objects[Lnum]->x - myx;
				dy = Info->objects[Lnum]->y - myy;
			}
			DoAction(Step, ACT_MOVE, dx, dy);
		}
	}
	else
	{
		DoAction(Step, ACT_TECH, 0,0.7*Info->robots[id]->L,0.3*Info->robots[id]->L);
		double Eint = -1;
		int Enum = -1;
		for (int i = 0; i<Info->field->Ne; i++)
		{
			int ex = Info->objects[i]->x;
			int ey = Info->objects[i]->y;
			double cint = sqrt(pow(ex-myx, 2) + pow(ey-myy, 2));
			if (cint < Eint || Eint == -1)
			{
				Eint = cint;
				Enum = i;
			}
		}
		double step = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;
		int dx;
		int dy;
		if (Eint > step)
		{
			dx = (Info->objects[Enum]->x - myx)*step/Eint;
			dy = (Info->objects[Enum]->y - myy)*step/Eint;
		}
		else
		{
			dx = Info->objects[Enum]->x - myx;
			dy = Info->objects[Enum]->y - myy;
		}
		DoAction(Step, ACT_MOVE, dx, dy);
	}
	return;
}


