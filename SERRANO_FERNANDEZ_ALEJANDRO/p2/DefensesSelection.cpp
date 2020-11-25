// ###### Config options ################


// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

using namespace Asedio;

double asignar_valor(const Defense& d){
    double x= d.range/d.damage;
    return x;
}


void DEF_LIB_EXPORTED selectDefenses(std::list<Defense*> defenses, unsigned int ases, std::list<int> &selectedIDs
            , float mapWidth, float mapHeight, std::list<Object*> obstacles) {


	int N = defenses.size(), i = 0;	//Capacidad total de defensas
	int p[N];
	double v[N], f[N][ases+1];
	

	//Asignar valores
	for(std::list<Defense*>::iterator it = defenses.begin(); it != defenses.end(); it++)
	{
		v[i] = asignar_valor(*(*it));
		p[i] = (*it)->cost;
		i++;
	}



	//Algoritmo que determina el máximo beneficio a obtener
	for(int j = 0; j <= ases; j++)
	{
		if(j < p[0])
			f[0][j] = 0;
		else
			f[0][j] = v[0];
	}



	for(i = 1; i < N; i++)
	{
		for(int j = 0; j <= ases ; j++)
		{
			if(j < p[i])
				f[i][j] = f[i-1][j];
				
			else		
				f[i][j] = std::max(f[i-1][j],f[i-1][j - p[i]]+v[i]);
		}
	}

	//Algoritmo que recupera la combinación óptima de defensas
	int j = ases;
	i = N-1;

    	std::list<Defense*>::iterator it = defenses.begin();

	//Colocamos la primera defensa
	selectedIDs.push_back((*it)->id);
	j = j - p[0];
	
	//Empezamos desde la última defensa
	it = defenses.end();
    	it--;
    	
	while(i != 0 && j != 0)
	{	
		if(f[i][j] != f[i-1][j])
		{					
			selectedIDs.push_back((*it)->id);
			j = j - p[i];					
		}
		i = i - 1;
		it--;
	}
	
	if(p[i] <= j)
		selectedIDs.push_back((*it)->id);

	/*	
	unsigned int cost = 0;
    	std::list<Defense*>::iterator it = defenses.begin();
 	while(it != defenses.end()) {
		if(cost + (*it)->cost <= ases) {
		    selectedIDs.push_back((*it)->id);
		    cost += (*it)->cost;
		}
		++it;
    	}	
		
	*/
	
}

