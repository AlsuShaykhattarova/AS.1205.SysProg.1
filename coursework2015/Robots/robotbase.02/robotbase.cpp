// robotbase.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "robotbase.h"

using namespace std;

extern "C" 
	int flag=0;

struct robo 
{
	CString name;
	bool party;
	double lengthTo;
	double prot;
	int E;
	bool live;
	int i;
	int x,y;
};

int coef=0;

int shag(int myX, int x2,int myV,int fVmax, int myE, int fLmax, int fEmax,double len2)
{
	int shaga=abs(myX - x2)*(myV*fVmax*myE/(fLmax*fEmax)) / len2;
	if (myX - x2 < 0){
					shaga = shaga;
				} else{
					shaga = -shaga;
				}
	return shaga;

}
	
void sort(robo *robots, int rivals){
		CString tmpname;
	bool tmpparty;
	bool tmplive;
	double tmplengthTo;
	double tmpprot;
	int tmpE,tmpi,tmpx,tmpy;
	for(int i = 0; i < rivals - 2; ++i) 
    {            
        for(int j = 0; j < rivals - 2; ++j)
        {     
            if (robots[j + 1].lengthTo < robots[j].lengthTo) 
            {
				tmpname=robots[j + 1].name;
				tmpparty=robots[j + 1].party;
                tmplengthTo = robots[j + 1].lengthTo; 
				tmpprot=robots[j + 1].prot;
				tmpE=robots[j + 1].E;
				tmplive=robots[j + 1].live;
				tmpi=robots[j + 1].i;
				tmpx=robots[j + 1].x;
				tmpy=robots[j + 1].y;

                robots[j + 1].name = robots[j].name;
                robots[j + 1].party = robots[j].party;
                robots[j + 1].lengthTo = robots[j].lengthTo;
                robots[j + 1].prot = robots[j].prot;
                robots[j + 1].E = robots[j].E;
                robots[j + 1].live = robots[j].live;
                robots[j + 1].i = robots[j].i;
                robots[j + 1].x = robots[j].x;
                robots[j + 1].y = robots[j].y;


                robots[j].name = tmpname;
                robots[j].party = tmpparty;
                robots[j].lengthTo = tmplengthTo;
                robots[j].prot = tmpprot;
                robots[j].E = tmpE;
                robots[j].live = tmplive;
                robots[j].i = tmpi;
                robots[j].x = tmpx;
                robots[j].y = tmpy;
            }
        }
    }
}

void DoStep(stepinfo *Info, step *Step)
{
	srand(time(0));
	int myNumber = Info->yourNumber;
	int myA=Info->robots[myNumber]->A;
	int myE=Info->robots[myNumber]->E;
	int myV=Info->robots[myNumber]->V;
	int fLmax=Info->field->Lmax;
	int fEmax=Info->field->Emax;
	int fVmax=Info->field->Vmax;
	int myX = Info->robots[myNumber]->x;
	int myY = Info->robots[myNumber]->y;
	int rivals=Info->field->rivals;
	robo *robots;
	CString target;
	robots=new robo[rivals-1]; 
	CString ali[12];
	ali[0]="robotbase.09";
	ali[1]="robotbase.09.1";
	ali[2]="robotbase.09.2";
	ali[3]="robotbase.09.3";
	ali[4]="robotbase.09.4";
	ali[5]="robotbase.09.5";
	ali[6]="robotbase.18";
	ali[7]="robotbase.18.1";
	ali[8]="robotbase.18.2";
	ali[9]="robotbase.18.3";
	ali[10]="robotbase.18.4";
	ali[11]="robotbase.18.5";


	for (int i = 0; i < rivals-1; i++)
	{
		robots[i].name=Info->robots[i]->name;
		if ((Info->robots[i]->name=="robotbase.02")||(Info->robots[i]->name=="robotbase.02.1")||(Info->robots[i]->name=="robotbase.02.2")||(Info->robots[i]->name=="robotbase.02.3")||(Info->robots[i]->name=="robotbase.02.4")||(Info->robots[i]->name=="robotbase.02.5"))
		{
			robots[i].lengthTo=-66;
		} else {
			robots[i].lengthTo=sqrt(pow(abs(myX - Info->robots[i]->x), 2) + pow(abs(myY - Info->robots[i]->y), 2));
		}



		if ((Info->robots[i]->name==ali[0])||(Info->robots[i]->name==ali[1])||
			(Info->robots[i]->name==ali[2])||(Info->robots[i]->name==ali[3])||
			(Info->robots[i]->name==ali[4])||(Info->robots[i]->name==ali[5])||
			(Info->robots[i]->name==ali[6])||(Info->robots[i]->name==ali[7])||
			(Info->robots[i]->name==ali[8])||(Info->robots[i]->name==ali[9])||
			(Info->robots[i]->name==ali[10])||(Info->robots[i]->name==ali[11])) 
		{robots[i].party=true;} else {robots[i].party=false;} 
		robots[i].prot=(1-Info->field->rndmax)*(Info->robots[i]->P)*(Info->robots[i]->E)/fEmax;
		robots[i].E=(Info->robots[i]->E);
		robots[i].live=(Info->robots[i]->alive);
		robots[i].i=i;
		robots[i].x=(Info->robots[i]->x);
		robots[i].y=(Info->robots[i]->y);

	}
	

void sort(robo robots, int rivals);

	double minprot=666666;
	double targetLen;
	int targetX, targetY, targetI;
	int ip=4;
		for(int i = 0; i < ip; i++) 
    {  
		if (robots[i].party) ip++;
		if (!robots[i].live) ip++;
		if (robots[i].lengthTo==-66) ip++;
		if ((robots[i].E<100)&&(!robots[i].party)&&(robots[i].lengthTo!=-66)&&(robots[i].live)) {
			minprot=robots[i].prot;
			target=robots[i].name;
			targetLen=robots[i].lengthTo;
			targetX=robots[i].x;
			targetY=robots[i].y;
			targetI=robots[i].i;
		} 
		else if ((robots[i].prot<minprot)&&(!robots[i].party)&&(robots[i].lengthTo!=-66)&&(robots[i].live)) {
			minprot=robots[i].prot;
			target=robots[i].name;
			targetLen=robots[i].lengthTo;
			targetX=robots[i].x;
			targetY=robots[i].y;
			targetI=robots[i].i;

		}
		if (target=="") ip++;
		if (ip>Info->field->rivals) ip=rivals;

	}

		double len2=1000000;
		double len3=1000000;
		int x2,y2,x3,y3;

		int ti=0;
		int tj=0;
		bool dead;
		for (int i = 0; i < ((Info->field->Ne)+(Info->field->Nl)); i++)
		{
			dead=1;
			for (int j = 0; j < Info->field->rivals; j++)
			{
				if ((Info->robots[j]->x==Info->objects[i]->x)&&(Info->robots[j]->y==Info->objects[i]->y)&&!(Info->robots[j]->alive)) dead=0;
			}
			if ((Info->objects[i]->type==-2)&&(dead!=0)) {

				double lenghtob2=sqrt(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2));

				if (lenghtob2<len2) {
					len2=lenghtob2;
					x2=Info->objects[i]->x;
					y2=Info->objects[i]->y;
				}
		
			}
			if ((Info->objects[i]->type==-3)&&(dead!=0)) {

				double lenghtob3=sqrt(pow(abs(myX - Info->objects[i]->x), 2) + pow(abs(myY - Info->objects[i]->y), 2));

				if (lenghtob3<len3) {
					len3=lenghtob3;
					x3=Info->objects[i]->x;
					y3=Info->objects[i]->y;
				}
	
			}

		}

for (int i = 0; i < rivals-1; i++)
{
		if(Info->robots[i]->name==target) targetI=i;
}


	
if(target==""){
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len2<=(myV*fVmax*myE/(fLmax*fEmax)))
		{

			DoAction(Step, ACT_MOVE,x2-myX, y2-myY);

		} else {
								
				DoAction(Step, ACT_MOVE,shag(myX,x2,myV,fVmax,myE,fLmax,fEmax,len2),shag(myY,y2,myV,fVmax,myE,fLmax,fEmax,len2));
		
		}
		

		
	}
else
if((Info->stepnum)<0.990*(Info->field->N))
{

	if ((myE>0.80*fEmax)&&((Info->robots[myNumber]->L)>0.80*fLmax)){
		
		if (targetLen<=(myV*fVmax*myE/(fLmax*fEmax)))
		{
			DoAction(Step, ACT_MOVE,targetX-myX-1, targetY-myY-1);
			if (((Info->field->rndmax)*myA*myE/fEmax)>((1-Info->field->rndmax)*(Info->robots[targetI]->P)*(Info->robots[targetI]->E)/fEmax)){
				if((Info->field->Rmax)*myV*myE/(fEmax*fLmax))
				{
					DoAction(Step, ACT_ATTACK,targetX-myX, targetY-myY);
					coef=0;
				}
			} else {
				DoAction(Step, ACT_TECH, (coef+0.45)*(Info->robots[myNumber]->L), (0.20-2*coef)*(Info->robots[myNumber]->L), (coef+0.35)*(Info->robots[myNumber]->L));
				coef=coef+0.05;
			}
		} else {
				DoAction(Step, ACT_TECH, (0.45)*(Info->robots[myNumber]->L), (0.20)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));
				DoAction(Step, ACT_MOVE,shag(myX,targetX,myV,fVmax,myE,fLmax,fEmax,targetLen),shag(myY,targetY,myV,fVmax,myE,fLmax,fEmax,targetLen));

		}
		
	
		
	} else if ((Info->robots[myNumber]->L)<0.80*fLmax) {
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len3<=(myV*fVmax*myE/(fLmax*fEmax)))
		{

			DoAction(Step, ACT_MOVE,x3-myX, y3-myY);

		} else {
			DoAction(Step, ACT_MOVE,shag(myX,x3,myV,fVmax,myE,fLmax,fEmax,len3),shag(myY,y3,myV,fVmax,myE,fLmax,fEmax,len3));
		}
		
		
	}  else if (myE<0.80*fEmax) {
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len2<=(myV*fVmax*myE/(fLmax*fEmax)))
		{

			DoAction(Step, ACT_MOVE,x2-myX, y2-myY);

		} else {
				DoAction(Step, ACT_MOVE,shag(myX,x2,myV,fVmax,myE,fLmax,fEmax,len2),shag(myY,y2,myV,fVmax,myE,fLmax,fEmax,len2));		
		}
				
	}
} else {
		DoAction(Step, ACT_TECH, 0, (0.65)*(Info->robots[myNumber]->L), (0.35)*(Info->robots[myNumber]->L));

		if (len2<=(myV*fVmax*myE/(fLmax*fEmax)))
		{

			DoAction(Step, ACT_MOVE,x2-myX, y2-myY);

		} else {
				
				DoAction(Step, ACT_MOVE,shag(myX,x2,myV,fVmax,myE,fLmax,fEmax,len2),shag(myY,y2,myV,fVmax,myE,fLmax,fEmax,len2));
				
		
		}
		

		
	}
	delete []robots; 

	return;
}


