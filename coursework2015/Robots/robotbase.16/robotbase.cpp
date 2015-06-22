// robotbase.cpp: определ€ет экспортированные функции дл€ приложени€ DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;
extern "C"

/*gd*/struct robo
{

	int id;
	double lengthTo;
	double prot;
	int E;
	bool live;
	int i;
	int x, y;
};

/*gd*/void kv_go(int bb, int  go_x, int  go_y, step *Step, stepinfo *Info)
{
	int my_id = Info->yourNumber;
	int my_x = Info->robots[my_id]->x;
	int my_y = Info->robots[my_id]->y;

	if ((go_y > my_y) && (go_x > my_x)){ DoAction(Step, ACT_MOVE, bb, bb); }
	if ((go_y < my_y) && (go_x < my_x)){ DoAction(Step, ACT_MOVE, -bb, -bb); }
	if ((go_y > my_y) && (go_x < my_x)){ DoAction(Step, ACT_MOVE, -bb, bb); }
	if ((go_y > my_y) && (go_x > my_x)){ DoAction(Step, ACT_MOVE, bb, -bb); }
}


/*gd*/void go_in(int  go_x, int  go_y, step *Step, stepinfo *Info)
{
	int move = (int)((Info->field->Vmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//дальность шага
	int my_id = Info->yourNumber;
	int my_x = Info->robots[my_id]->x;
	int my_y = Info->robots[my_id]->y;


	int DX = abs(go_x - my_x); int DY = abs(go_y - my_y); int _;
	if (((abs(my_x - go_x)) == (abs(my_y - go_y))) && (((abs(my_y - go_y)) != 0) && ((abs(my_x - go_x)) != 0)))
	{
		if (int(sqrt(move)) > DX) { _ = DX; kv_go(_, go_x, go_y, Step, Info); }
		if ((int(sqrt(abs(move)))) == DX){ _ = move; kv_go(_, go_x, go_y, Step, Info); }
		if (int(sqrt(move)) < DX){ _ = int(abs(sqrt(move))); kv_go(_, go_x, go_y, Step, Info); }
	}
	//21
	if (DX > DY)
	{
		if (go_x > my_x)//21'
		{
			if (DX<move){ _ = DX; DoAction(Step, ACT_MOVE, _, 0); }
			if (DX == move){ _ = ((move)); DoAction(Step, ACT_MOVE, _, 0); }
			if (DX >move){ _ = ((move)); DoAction(Step, ACT_MOVE, _, 0); }
		}
		if (go_x < my_x)//21''
		{
			if (DX<move){ _ = (-DX); DoAction(Step, ACT_MOVE, _, 0); }
			if (DX == move){ _ = (-(move)); DoAction(Step, ACT_MOVE, _, 0); }
			if (DX > move){ _ = (-(move)); DoAction(Step, ACT_MOVE, _, 0); }
		}
	}
	if (DX < DY)
	{
		if (go_y > my_y)
		{
			if (DY<((abs(move))))
			{
				_ = DY; DoAction(Step, ACT_MOVE, 0, _);
			}
			if (DY == ((abs(move)))){ _ = ((move)); DoAction(Step, ACT_MOVE, 0, _); }
			if (DY >((abs(move)))){ _ = ((move)); DoAction(Step, ACT_MOVE, 0, _); }
		}
		if (go_y < my_y)
		{
			if (DY<((abs(move)))){ _ = (-DY); DoAction(Step, ACT_MOVE, 0, _); }
			if (DY == ((abs(move)))){ _ = (-(move)); DoAction(Step, ACT_MOVE, 0, _); }
			if (DY >((abs(move)))){ _ = (-(move)); DoAction(Step, ACT_MOVE, 0, _); }
		}
	}




}

/*gd*/double rastoyanie(int x1, int y1, stepinfo *Info)
{
	int my_id = Info->yourNumber;
	int my_x = Info->robots[my_id]->x;
	int my_y = Info->robots[my_id]->y;
	int W = Info->field->fieldWidth;
	int H = Info->field->fieldHeight;

	double s1, s2, s3, s4;
	s1 = sqrt((x1 - my_x)*(x1 - my_x) + (y1 - my_y)*(y1 - my_y));
	s2 = sqrt(pow(abs(x1 - my_x) - W, 2) + (y1 - my_y)*(y1 - my_y));
	s3 = sqrt(pow(abs(y1 - my_y) - H, 2) + (x1 - my_x)*(x1 - my_x));
	s4 = sqrt(pow(abs(y1 - my_y) - H, 2) + pow(abs(x1 - my_x) - W, 2));
	if (s1 > s2) s1 = s2;
	if (s3 > s4) s3 = s4;
	if (s1 > s3) s1 = s3;
	return s1;
}

/*gd*/void go_in_blzg_energi(stepinfo *Info, step *Step)
{
	int move = (int)((Info->field->Vmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//дальность шага
	int my_id = Info->yourNumber;
	int my_x = Info->robots[my_id]->x;
	int my_y = Info->robots[my_id]->y;
	int n_Ne = Info->field->Ne;
	int n_Nl = Info->field->Nl;

	double se = rastoyanie(Info->objects[0]->x, Info->objects[0]->y, Info);

	double xe = Info->objects[0]->x;
	double ye = Info->objects[0]->y;
	int ne = 0;
	for (int i1 = 1; i1 < n_Ne; i1++)
	{
		if (rastoyanie(Info->objects[i1]->x, Info->objects[i1]->y, Info) < se)
		{
			se = rastoyanie(Info->objects[i1]->x, Info->objects[i1]->y, Info);
			ne = i1;
			xe = Info->objects[i1]->x;
			ye = Info->objects[i1]->y;
		}
	}
	go_in(xe, ye, Step, Info);
}

/*gd*/void go_in_blzg_hpresp(stepinfo *Info, step *Step)
{
	int move = (int)((Info->field->Vmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//дальность шага
	int n_Ne = Info->field->Ne;
	int n_Nl = Info->field->Nl;

	int Nl = Info->field->Nl;
	double sl = rastoyanie(Info->objects[0]->x, Info->objects[0]->y, Info);
	double xh = Info->objects[n_Nl]->x;
	double yh = Info->objects[n_Nl]->y;
	int nl = n_Nl;
	for (int i2 = n_Nl + 1; i2 < n_Nl + Nl; i2++)
	{
		if (rastoyanie(Info->objects[i2]->x, Info->objects[i2]->y, Info) < sl)
		{
			sl = rastoyanie(Info->objects[i2]->x, Info->objects[i2]->y, Info);
			nl = i2;
			xh = Info->objects[i2]->x;
			yh = Info->objects[i2]->y;
		}
	}
	go_in(xh, yh, Step, Info);
}

//warning C4715: mugu_srazu_ubit: значение возвращаетс€ не при всех пут€х выполнени€	c:\users\vladilen\desktop\as.1204.sysprog.2-master (1)\as.1204.sysprog.2-master\coursework2015\robots\robotbase.18\robotbase.cpp	178	1	robotbase.18
/**/BOOL mugu_srazu_ubit(stepinfo *Info)
{
	int Lmax = Info->field->Lmax;
	int my_id = Info->yourNumber;
	double RNDmin = Info->field->rndmin;
	int A = Info->robots[my_id]->A;
	int E = Info->robots[my_id]->E;
	int Emax = Info->field->Emax;
	double min_A = RNDmin * A * E / Emax; // минимальна€ возможна€ атака моего робота


	for (int i3 = 1; i3 < (Info->field->rivals); i3++)
	{
		if ((Info->robots[i3]->alive) && ((Info->robots[i3]->playerid) != my_id))
		{
			double max_drP = (1 - RNDmin) * (Info->robots[i3]->P) * (Info->robots[i3]->E) / Emax;
			if (((max_drP)) <= min_A)
			{	//100% смертники
				if (5 * (abs(min_A - max_drP)*Emax / Lmax) > Info->robots[i3]->E)
				{

					return TRUE;
					//attac(i, Info, radattack, move, Step,  rast_A)
				}
			}

		}
		return FALSE;
	}

}

/*raspredelenia*/void attac(int ss, stepinfo *Info, step *Step)
{
	double radattack = abs((Info->field->Rmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//радиус удара
	int move = (int)((Info->field->Vmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//дальность шага
	int my_id = Info->yourNumber;
	int my_x = Info->robots[my_id]->x;
	int my_y = Info->robots[my_id]->y;

	int x, y;
	x = (Info->robots[ss]->x);
	y = (Info->robots[ss]->y);
	double rast_A = abs(rastoyanie(x, y, Info)-3);
	if (rast_A >= radattack)
	{
		int aa, ss;
		aa = x - int(sqrt(radattack));
		ss = y - int(sqrt(radattack));
		go_in(aa, ss, Step, Info);
	}
	else{
		if (((Info->robots[ss]->x) == x) && ((Info->robots[ss]->y) == y) && (Info->robots[ss]->alive))
		{
			int xx, yy;
			//ubral modul

			DoAction(Step, ACT_ATTACK, x - my_x, y - my_y);
		}
	}

}

/**/int nah_bl_rb(stepinfo *Info, step *Step)
{
	robo *dan; dan = new robo[Info->field->rivals - 1];
	double cvc = 1000; int id_bliz_robota;
	for (int i = 0; i < (Info->field->rivals - 1); i++)
	{
		dan[i].live = (Info->robots[i]->alive);
		dan[i].id = Info->robots[i]->playerid;
		dan[i].lengthTo = sqrt(pow(abs(Info->robots[Info->yourNumber]->x - Info->robots[i]->x), 2) + pow(abs(Info->robots[Info->yourNumber]->y - Info->robots[i]->y), 2));
		if ((Info->robots[i]->alive) && (dan[i].lengthTo < cvc) && (dan[i].lengthTo>0.1) && (Info->yourNumber != Info->robots[i]->playerid))
			                                                        //     blackhole   //
		{
			cvc = dan[i].lengthTo; id_bliz_robota = dan[i].id;
		}

	}
	return id_bliz_robota;
	
}


int chel(stepinfo *Info, step *Step)
{
	robo *dan; dan = new robo[Info->field->rivals - 1];
	int lol;

	for (int i = 0; i < (Info->field->rivals - 1); i++)
	{
		dan[i].id = Info->robots[i]->playerid;
		dan[i].live = (Info->robots[i]->alive);
		dan[i].lengthTo = sqrt(pow(abs(Info->robots[Info->yourNumber]->x - Info->robots[i]->x), 2) + pow(abs(Info->robots[Info->yourNumber]->y - Info->robots[i]->y), 2));
		dan[i].E = (Info->robots[i]->E);
		dan[i].prot = (1 - Info->field->rndmax)*(Info->robots[i]->P)*(Info->robots[i]->E) / (Info->field->Emax);
		if (Info->yourNumber != Info->robots[i]->playerid)
		{
			if (dan[i].live)
			{
				if ((dan[i].E <= (0.5*(Info->field->Emax))) || (FALSE))
				{
					int n_Ne = Info->field->Ne; double rastEN = 100000;
					for (int ms = 1; ms < n_Ne; ms++)
					{
						if (rastoyanie(Info->objects[ms]->x, Info->objects[ms]->y, Info) < rastEN)
						{
							rastEN = abs(rastoyanie(Info->objects[ms]->x, Info->objects[ms]->y, Info));
						}

					}
					int n_Nl = Info->field->Nl; double rastHP = 100000;
					for (int mm = 1; mm < n_Nl; mm++)
					{
						if (rastoyanie(Info->objects[mm]->x, Info->objects[mm]->y, Info) < rastHP)
						{
							rastHP = abs(rastoyanie(Info->objects[mm]->x, Info->objects[mm]->y, Info));
						}

					}
					//ne uchtenorastoyanie
					return dan[i].id;



				}


			}



		}


	}
	delete[]dan;

}

BOOL i_m_hp_resp(stepinfo *Info)
{
	int n_Nl = Info->field->Nl; 
	for (int mm = 1; mm < n_Nl; mm++)
	{
		if (rastoyanie(Info->objects[mm]->x, Info->objects[mm]->y, Info) == 0)
		{
			return true;
		}
		else return false;
	}


}

BOOL i_m_en_resp(stepinfo *Info)

{
	int n_Ne = Info->field->Ne;
	for (int mm = 1; mm < n_Ne; mm++)
	{
		if (rastoyanie(Info->objects[mm]->x, Info->objects[mm]->y, Info) == 0)
		{
			return true;
		}
		else return false;
	}

}


void DoStep(stepinfo *Info, step *Step)
{


	



	srand(time(0));
	int move = (int)((Info->field->Vmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//дальность шага

	int my_e = Info->robots[Info->yourNumber]->E;//мое значение энергии
	int my_res = Info->robots[Info->yourNumber]->L;//тех сост текущее
	int my_max_e = Info->field->Emax;
	int res = Info->field->Lmax;
	double krit = 0.90;
	double dost = 0.500;
	double zaryad = 0.97;
	double radattack = abs((Info->field->Rmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//радиус удара
	
	if ((i_m_hp_resp(Info)) && ((my_res > dost*res)))
	{
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));

		go_in_blzg_energi(Info, Step); return;
		

	}
	
	
	if  ((i_m_en_resp(Info)) && ((my_e > dost*my_max_e)))
	{
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));

		go_in_blzg_hpresp(Info, Step); return;


	}
	


	if (my_res < krit*res)
	{
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));
		go_in_blzg_hpresp(Info, Step);
		
		return;
	}
	if (my_e < krit*my_max_e)
	{
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));
		go_in_blzg_energi(Info, Step);
		return;
	}

	if (Info->stepnum < 10)
	{
		go_in_blzg_hpresp(Info, Step);
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));
	}


	///


	/*

	if (i_m_hp_resp(Info))

	{if ((Info->robots[Info->yourNumber]->L) >= (zaryad*(Info->field->Lmax)))

{

			if ((Info->robots[Info->yourNumber]->E) >= (dost*(Info->field->Emax)))

			{
				
				DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.45), (Info->robots[Info->yourNumber]->L)*(0.45), (Info->robots[Info->yourNumber]->L)*(0.10));

				attac(nah_bl_rb(Info, Step), Info, Step); return;

			}

			else

			{
				DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.70), (Info->robots[Info->yourNumber]->L)*(0.30)); return;

				go_in_blzg_energi(Info, Step); return;

			}

		}

		else
		{
		
			if (abs(rastoyanie(Info->robots[nah_bl_rb(Info, Step)]->x, Info->robots[nah_bl_rb(Info, Step)]->y, Info)) <= radattack)
			{
				DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.60), (Info->robots[Info->yourNumber]->L)*(0.40), (Info->robots[Info->yourNumber]->L)*(0.00)); return;

				attac(nah_bl_rb(Info, Step), Info, Step); return;
			}
		
		}

	}

	else

	{

		if ((Info->robots[Info->yourNumber]->L) <= (krit*(Info->field->Lmax)))

		{
			DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.70), (Info->robots[Info->yourNumber]->L)*(0.30)); 

			go_in_blzg_hpresp(Info, Step); return;

		}

		else

		{

			if (i_m_en_resp(Info))

			{

				if ((Info->robots[Info->yourNumber]->E) >= (zaryad*(Info->field->Emax)))

				{

					if ((Info->robots[Info->yourNumber]->L) >= (dost*(Info->field->Lmax)))

					{

						DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.45), (Info->robots[Info->yourNumber]->L)*(0.45), (Info->robots[Info->yourNumber]->L)*(0.10));

						attac(nah_bl_rb(Info, Step), Info, Step); return;

					}

					else

					{
						DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.70), (Info->robots[Info->yourNumber]->L)*(0.30)); 

						go_in_blzg_hpresp(Info, Step); return;

					}

				}

				else{
				
					if (abs(rastoyanie(Info->robots[nah_bl_rb(Info, Step)]->x, Info->robots[nah_bl_rb(Info, Step)]->y, Info)) <= radattack)
					{
						DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.60), (Info->robots[Info->yourNumber]->L)*(0.40), (Info->robots[Info->yourNumber]->L)*(0.0)); 

						attac(nah_bl_rb(Info, Step), Info, Step); return;
					}
				
				}

			}

			else

			{

				if ((Info->robots[Info->yourNumber]->E) <= (krit*(Info->field->Emax)))

				{
					DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.70), (Info->robots[Info->yourNumber]->L)*(0.30)); 

					go_in_blzg_energi(Info, Step); return;

					

				}

				else

				{

					if (my_res < krit*res)
					{
						go_in_blzg_hpresp(Info, Step);
						DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));
						return;
					}
					if (my_e < krit*my_max_e)
					{
						go_in_blzg_energi(Info, Step);
						DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));
						return;
					}


					
					attac(nah_bl_rb(Info, Step), Info, Step); return;




				}

			}

		}

	}

	///////////////////////////////////////DEREVO/////////////////////////////////////////////////////////






	*/


	//////////////
	/*
	if (Info->stepnum < 10)
	{
		go_in_blzg_hpresp(Info, Step);
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));
	}

	if (my_res < krit*res)
	{
		go_in_blzg_hpresp(Info, Step);
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));

	}
	if (my_e < krit*my_max_e)
	{
		go_in_blzg_energi(Info, Step);

	}
	
	DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.00), (Info->robots[Info->yourNumber]->L)*(0.65), (Info->robots[Info->yourNumber]->L)*(0.35));

	*/


	////////////////



	///////////////////////////////////////DEREVO/////////////////////////////////////////////////////////
	/*
	if ((my_res > krit*res) && (my_e > krit*my_max_e))
	{
		attac(nah_bl_rb(Info, Step), Info, Step);
	}
	else
	{
		if (my_e < krit*my_max_e){ go_in_blzg_energi(Info, Step); }
		if (my_res > krit*res){ go_in_blzg_hpresp(Info, Step); }
	
	}
	*/
	///////////////////////////////////////DEREVO/////////////////////////////////////////////////////////


	
	
	/*
	
	
	
	attac(nah_bl_rb(Info, Step), Info, Step);
	if ((i_m_hp_resp(Info)) || (i_m_en_resp(Info)) )
	{
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.50), (Info->robots[Info->yourNumber]->L)*(0.35), (Info->robots[Info->yourNumber]->L)*(0.15));
	}

	
	
	if (my_res < krit*res)
	{
		go_in_blzg_hpresp(Info, Step);
		
	}


	if (my_e < krit*my_max_e)
	{
		go_in_blzg_energi(Info, Step);

	}

	if ((my_e > dost*my_max_e) && (my_res > dost*res))
	{
		DoAction(Step, ACT_TECH, (Info->robots[Info->yourNumber]->L)*(0.50), (Info->robots[Info->yourNumber]->L)*(0.35), (Info->robots[Info->yourNumber]->L)*(0.15));

		attac(nah_bl_rb(Info, Step), Info, Step);
	}
	else
	{
		attac(nah_bl_rb(Info, Step), Info, Step);
	}



	*/


	/*

	if (Info->stepnum < 10)
	{go_in_blzg_hpresp(Info, Step);
	DoAction(Step, ACT_TECH, 60, 40, 00);
	if (abs(rastoyanie(Info->robots[nah_bl_rb(Info, Step)]->x, Info->robots[nah_bl_rb(Info, Step)]->y, Info)) <= radattack)
	{
		DoAction(Step, ACT_TECH, 60, 40, 00);
		attac(nah_bl_rb(Info, Step), Info, Step);
	}
	}
	*/
	

///////////////////////////////////////DEREVO/////////////////////////////////////////////////////////
	/*
	if (Info->stepnum < 10)
	{
		go_in_blzg_hpresp(Info, Step);

		DoAction(Step, ACT_TECH, res* 0.40, res*0.60, res*0.0);
	}
		if ((my_e > krin*my_max_e) && (my_res>krin*res))
		{
			if (abs(rastoyanie(Info->robots[nah_bl_rb(Info, Step)]->x, Info->robots[nah_bl_rb(Info, Step)]->y, Info)) <= radattack)
			{
				attac(nah_bl_rb(Info, Step), Info, Step);
			}
			
		}
		*/
	/*
	if (my_e < krin*my_max_e){ go_in_blzg_energi(Info, Step); }
	else if (my_res < krin*res){ go_in_blzg_hpresp(Info, Step); }
	else 
	{ 
		
		attac(chel(Info, Step), Info, Step); 
	
	
	}
	*/
	//DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));
	/*	robo *dan; dan = new robo[Info->field->rivals - 1];

	for (int i = 0; i < (Info->field->rivals - 1); i++)
	{
	dan[i].id = Info->robots[i]->playerid;
	dan[i].x = (Info->robots[Info->robots[i]->playerid]->x);
	dan[i].y = (Info->robots[Info->robots[i]->playerid]->y);
	dan[i].prot = (1 - Info->field->rndmax)*(Info->robots[i]->P)*(Info->robots[i]->E) / (Info->field->Emax);
	dan[i].live = (Info->robots[i]->alive);
	dan[i].E = (Info->robots[i]->E);
	dan[i].lengthTo = sqrt(pow(abs(Info->robots[Info->yourNumber]->x - Info->robots[i]->x), 2) + pow(abs(Info->robots[Info->yourNumber]->y - Info->robots[i]->y), 2));

	}*/


	/*int myNumber = Info->yourNumber;
	int myX = Info->robots[Info->yourNumber]->x;
	int myY = Info->robots[Info->yourNumber]->y;*/
	//go_in(Info->robots[nah_bl_rb(Info, Step)]->x, Info->robots[nah_bl_rb(Info, Step)]->y, move, Step, Info);
	//nah_bl_rb(Info, Step);



	/*

	DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[Info->yourNumber]->L), (0.35)*(Info->robots[Info->yourNumber]->L));

	int move = (int)((Info->field->Vmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//дальность шага
	double radattack =abs( (Info->field->Rmax)*(Info->robots[Info->yourNumber]->V) / (Info->field->Lmax)*(Info->robots[Info->yourNumber]->E) / (Info->field->Emax));//радиус удара



	int my_e = Info->robots[Info->yourNumber]->E;//мое значение энергии
	int my_hp = Info->robots[Info->yourNumber]->L;//тех сост текущее
	int my_max_e = Info->field->Emax;
	int my_max_hp = Info->field->Lmax;



	double e_kr = 0.60;
	double hp_kr = 0.60;

	*/

	//	delete[]robots;
	//delete[]objects2;
	//delete[]objects3;
return;
}


