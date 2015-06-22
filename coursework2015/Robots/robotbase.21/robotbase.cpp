// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

double dlina(int x1, int y1, int x2, int y2, int H, int W)
{
	double s1,s2,s3,s4;
	s1 = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	s2 = sqrt(pow(abs(x1 - x2)-W,2) + (y1 - y2)*(y1 - y2));
	s3 = sqrt(pow(abs(y1 - y2) - H, 2) + (x1 - x2)*(x1 - x2));
	s4 = sqrt(pow(abs(y1 - y2) - H, 2) + pow(abs(x1 - x2) - W, 2));
	if (s1 > s2) s1 = s2;
	if (s3 > s4) s3 = s4;
	if (s1 > s3) s1 = s3;
	return s1;
}

int shag(int x, int Pi)
{
	int x1,x2;
	x1 = x + Pi;
	x2 = x - Pi;
	if (abs(x1) > abs(x2)) x1 = x2;
	if (abs(x) > abs(x1)) x = x1;
	return x;

}

struct point
{
	int x, y;
};


point max_pyt(int x1, int y1, int x2, int y2, double p_s_lim, int H, int W)
{
	point t;

	if (dlina(x1, y1, x2, y2, H, W) <= p_s_lim)
	{
		t.x = x2;
		t.y = y2;
		return t;
	}

	int znak_x=0, znak_y=0;

	if (abs(x1 - x2) > W / 2)
	{
		if (x1 <= W / 2) x1 = x1 + W;
		else x2 = x2 + W;
	}

	if (abs(y1 - y2) > H / 2)
	{
		if (y1 <= H / 2) y1 = y1 + H;
		else y2 = y2 + H;
	}

	double dolya_x, dolya_y;

	if (abs(x1 - x2)) znak_x = (x2 - x1) / abs(x2 - x1);
	if (abs(y1 - y2)) znak_y = (y2 - y1) / abs(y2 - y1);


	t.x = x1;
	t.y = y1;

	if (!(znak_x))
	{
		t.y = t.y + znak_y* int(p_s_lim);
	}

	if (!(znak_y))
	{
		t.x = t.x + znak_x* int(p_s_lim);
	}
	
	if ((znak_x) && (znak_y))
	{
		double s;

		dolya_x = abs(x2 - x1*1.0)/(abs(x2 - x1) + abs(y2 - y1));
		dolya_y = abs(y2 - y1*1.0)/(abs(x2 - x1) + abs(y2 - y1));

		s = p_s_lim / (sqrt(dolya_x*dolya_x + dolya_y*dolya_y));

		dolya_x = dolya_x *s;
		dolya_y = dolya_y *s;

		t.x = t.x + znak_x * int(dolya_x);
		t.y = t.y + znak_y * int(dolya_y);

		while (dlina(x1,y1,t.x,t.y,H,W)>=p_s_lim)
		{
			if (t.x != x1)
			{
				t.x = t.x - znak_x;
			}
			else
			{
				t.y = t.y - znak_y;
			}
		}


	}

	if (t.x >= W) t.x = t.x - W;

	if (t.y >= H) t.y = t.y - H;

	return t;

};



extern "C" 
void DoStep(stepinfo *Info, step *Step)
{
	srand(time(0));



	int n = Info->field->rivals;//Кол-во роботов на поле

	int myNumber = Info->yourNumber;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	int L = Info->robots[myNumber]->L;
	int A = Info->robots[myNumber]->A;
	int P = Info->robots[myNumber]->P;
	int V = Info->robots[myNumber]->V;
	int E = Info->robots[myNumber]->E;

	//параметры поля:
	int Lmax = Info->field->Lmax;
	int Emax = Info->field->Emax;
	int Vmax = Info->field->Vmax;
	int Rmax = Info->field->Rmax;
	int W = Info->field->fieldWidth;
	int H = Info->field->fieldHeight;



	double p_s_lim = Vmax*V*E / (Lmax*Emax); // передел перемещения
	
	//Поиск ближайшей кормушки:
	int Ne = Info->field->Ne;
	double se = dlina(Info->objects[0]->x, Info->objects[0]->y, myX, myY, H, W);
	double xe = Info->objects[0]->x;
	double ye = Info->objects[0]->y;
	int ne = 0;
	for (int i = 1; i < Ne; i++)
	{
		if (dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W) < se)
		{
			se = dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W);
			ne = i;
			xe = Info->objects[i]->x;
			ye = Info->objects[i]->y;
		}
	}


	//Поиск ближайшей мастерской:
	int Nl = Info->field->Nl;
	double sl = dlina(Info->objects[0]->x, Info->objects[0]->y, myX, myY, H, W);
	double xl = Info->objects[Ne]->x;
	double yl = Info->objects[Ne]->y;
	int nl = Ne;
	for (int i = Ne+1; i < Ne + Nl; i++)
	{
		if (dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W) < sl)
		{
			sl = dlina(Info->objects[i]->x, Info->objects[i]->y, myX, myY, H, W);
			nl = i;
			xl = Info->objects[i]->x;
			yl = Info->objects[i]->y;
		}
	}

	//если требуется подзарядка
	if (E < 0.9*Emax)
	{
		if (se <= p_s_lim)
		{
			if (V == 0)
			{
				DoAction(Step, ACT_TECH, 0.2*L, 0.4*L, 0.4*L);
				return;
			}

			DoAction(Step, ACT_MOVE, shag(xe - myX, W), shag(ye - myY, H));
			if (P != L)
				DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			return;
		}
		else
		{
			if (V == 0)
			{
				DoAction(Step, ACT_TECH, 0.2*L, 0.4*L, 0.4*L);
				return;
			}

			if (L < 0.70*Lmax)
			{
				if (sl < p_s_lim)
				{
					if (sl)
						DoAction(Step, ACT_MOVE, shag(xl - myX, W), shag(yl - myY, H));
					if ((P != 0.6*L) || (V != 0.4*L))
						DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
					return;
				}
			}

			point t = max_pyt(myX, myY, xe, ye, p_s_lim, H, W);

			DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));

			if (L >= 0.4*Lmax)
			{
				if ((P != 0.6*L) || (V != 0.4*L))
					DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			}
			else
			{
				DoAction(Step, ACT_TECH, 0, 0, L);
			}


			return;
		}


	}

	//если требуется на пункт ТО
	if ((L < 0.90*Lmax) && (V != 0))
	{
		if (sl < p_s_lim)
		{
			if (sl)
				DoAction(Step, ACT_MOVE, shag(xl - myX, W), shag(yl - myY, H));

			if (P != L)
				DoAction(Step, ACT_TECH, 0.6*L, 0.4*L, 0);

			return;
		}

		else
		{
			point t = max_pyt(myX, myY, xl, yl, p_s_lim, H, W);

			DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));

			if (L >= 0.4*Lmax)
			{
				if ((P != 0.6*L) || (V != 0.4*L))
					DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			}
			else
			{
				DoAction(Step, ACT_TECH, 0, 0, L);
			}

			return;
		}

	}



	//после 990 хода бежать на кормушку:
	if (Info->stepnum > 990)
	{
		if (se <= p_s_lim)
		{
			if (se != 0)
				DoAction(Step, ACT_MOVE, shag(xe - myX, W), shag(ye - myY, H));

			if (L >= 0.4*Lmax)
			{
				if ((P != 0.6*L) || (V != 0.4*L))
					DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
			}
			else
			{
				DoAction(Step, ACT_TECH, 0, 0, L);
			}

			return;
		}
		else
		{
			point t = max_pyt(myX, myY, xe, ye, p_s_lim, H, W);

			DoAction(Step, ACT_MOVE, shag(t.x - myX, W), shag(t.y - myY, H));

			if ((P != 0.6*L) || (V != 0.4*L))
				DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);

			return;
		}
	}

	
	if ((P != 0.6*L) || (V != 0.4*L))
	{
		DoAction(Step, ACT_TECH, 0, 0.6*L, 0.4*L);
	}

	DoAction(Step, ACT_MOVE, int(p_s_lim), 0);

	return;
}


