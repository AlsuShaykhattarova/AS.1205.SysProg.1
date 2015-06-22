// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	int id = Info->yourNumber;
	int fr1 = Info->robots[id]->playerid;
	int fr2 = fr1;
	for (int i = 0; i<Info->field->rivals; i++)
	{
		if (Info->robots[i]->name == "robotbase.04" || Info->robots[i]->name == "robotbase.04.1")
		{
			fr2 = Info->robots[i]->playerid;
			break;
		}
	}
	if (Info->robots[id]->L < Info->field->Lmax && Info->robots[id]->E > 0.8*Info->field->Emax)
	{
		double m_l1 = sqrt(pow(Info->objects[Info->field->Ne]->x-Info->robots[id]->x, 2) + pow(Info->objects[Info->field->Ne]->y-Info->robots[id]->y, 2));	
		int index = Info->field->Ne;
		if (Info->field->Nl > 1)
		{
			for (int i = 1; i<Info->field->Ne+Info->field->Nl; i++)
			{
				double c_l = sqrt(pow(Info->objects[i]->x-Info->robots[id]->x, 2) + pow(Info->objects[i]->y-Info->robots[id]->y, 2));
				if (c_l < m_l1)
				{
					m_l1 = c_l;
					index = i;
				}
			}
		}
		if (m_l1 > 0)	
		{
			double s = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;	
			double n = m_l1 / s;	
			int g_x = Info->objects[index]->x;	
			int g_y = Info->objects[index]->y;
			if (n > 1)	
			{
				int V = Info->field->Vmax;
				if (V > Info->robots[id]->L)
					V = Info->robots[id]->L;
				DoAction(Step, ACT_TECH, 0, Info->robots[id]->L-V, V);	
				int dx = g_x - Info->robots[id]->x;
				int dy = g_y - Info->robots[id]->y;
				dx/=n;
				dy/=n;
				DoAction(Step, ACT_MOVE, dx, dy);	
			}
			else	
			{
				int V = Info->robots[id]->L/2;
				if (V > Info->field->Vmax)
					V = Info->field->Vmax;
				DoAction(Step, ACT_TECH, 0, Info->robots[id]->L, 0);	
				int dx = g_x - Info->robots[id]->x;
				int dy = g_y - Info->robots[id]->y;
				DoAction(Step, ACT_MOVE, dx, dy); 
			}
		}
		else
			DoAction(Step, ACT_TECH, 0, Info->robots[id]->L, 0);
		return;
	}
	

	double m_l2 = sqrt(pow(Info->objects[0]->x-Info->robots[id]->x, 2) + pow(Info->objects[0]->y-Info->robots[id]->y, 2));	
	int index = 0;
	

	if (m_l2 > 0)	
	{
		double s = Info->field->Vmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;	
		double n = m_l2 / s;	
		int g_x = Info->objects[index]->x;	
		int g_y = Info->objects[index]->y;
		if (n > 1)	
		{
			int V = Info->field->Vmax;
			if (V > Info->robots[id]->L)
				V = Info->robots[id]->L;
			DoAction(Step, ACT_TECH, 0, Info->robots[id]->L-V, V);	
			int dx = g_x - Info->robots[id]->x;
			int dy = g_y - Info->robots[id]->y;
			dx/=n;
			dy/=n;
			DoAction(Step, ACT_MOVE, dx, dy);	
		}
		else	
		{
			int V = Info->robots[id]->L/2;
			if (V > Info->field->Vmax)
				V = Info->field->Vmax;
			DoAction(Step, ACT_TECH, Info->robots[id]->L-V, 1, V-1);	
			int dx = g_x - Info->robots[id]->x;
			int dy = g_y - Info->robots[id]->y;
			DoAction(Step, ACT_MOVE, dx, dy); 
		}
	}
	else  
	{
		int V = Info->robots[id]->L/2;		
		if (V > Info->field->Vmax)
			V = Info->field->Vmax;
		DoAction(Step, ACT_TECH, Info->robots[id]->L-V, 1, V-1);
		double r_a = Info->field->Rmax*Info->robots[id]->V/Info->field->Lmax*Info->robots[id]->E/Info->field->Emax;	
		int index = 0;
		int m_e = Info->field->Emax;
		bool g = false;
		for (int i = 0; i < Info->field->rivals; i++)
		{
			if (Info->robots[i]->playerid != fr1 && Info->robots[i]->playerid != fr2)
			{
				double c_l = sqrt(pow(Info->robots[i]->x-Info->robots[id]->x, 2) + pow(Info->robots[i]->y-Info->robots[id]->y, 2)); 
				if (c_l <= r_a && Info->robots[i]->E < m_e)	
				{
					m_e = Info->robots[i]->E;
					index = i;
					g = true;
				}
			}
		}
		if (g)	
		{
			int dx = Info->robots[index]->x - Info->robots[id]->x;
			int dy = Info->robots[index]->y - Info->robots[id]->y;
			//DoAction(Step, ACT_ATTACK, dx, dy);
			if (dx != 0 && dy != 0 && Info->robots[id]->A > 0)
			{
				DoAction(Step, ACT_ATTACK, dx, dy);
			}
		}
	}
	return;
}


