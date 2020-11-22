// ###### Config options ################


// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

using namespace Asedio;

double asignar_valor()
{

}

void DEF_LIB_EXPORTED selectDefenses(std::list<Defense*> defenses, unsigned int ases, std::list<int> &selectedIDs
            , float mapWidth, float mapHeight, std::list<Object*> obstacles) {


	int N = defenses.size(), i = 0;	//Capacidad total de defensas
	double p[N], v[N], f[N][ases];
	
	for(std::list<Defense*>::iterator i = defenses.begin(); i != defenses.end(); i++)
	{
		v[i] = asignar_valor();
		p[i] = (*i)->cost;
		i++;
	}

	for(int j = 0; i < ases; j++)
	{
		if(j < p[1])
			f[1][j] = 0;
		else
			f[1][j] = v[1];
	}



	//Algoritmo de la mochila
	for(int i = 2; i < N; i++)
	{
		for(int j = 0; j < ases ; j++)
		{
			if(j < p[i])
				f[i][j] = f[i-1][j];
				
			else		
				f[i][j] = max(f[i-1][j],f[i-1][j - p[i]]+v[i]);
		}
	}
	
	//Algoritmo ejercicio 2
	int j = 0;
	i = 0;
	
	while(i != 1 && j != 0)
	{
		if(f[i][j] != f[i-1][j])
		{
			//S <- S U O[i].peso
			j = j - O[i].peso
		}
		i = i - 1;
	}
	
	if(O[i].peso <= j)
		//S <- S U O[1]
}
