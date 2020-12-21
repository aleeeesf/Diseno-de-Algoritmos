// ###### Config options ################



// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"
#include <algorithm>

using namespace Asedio;        

struct Celda
{

		int row_, col_;
		float value_;
		Celda():row_(0),col_(0),value_(0.0f){}
		Celda(int fila, int columna, float valor):row_(fila),col_(columna),value_(valor){}
};

bool operator < (const Celda& a, const Celda& b){ return a.value_ < b.value_;}
bool operator <= (const Celda& a, const Celda& b){ return a.value_ <= b.value_;}



float distancia(Celda celdaActual,std::list<Defense*>::iterator objetoActual, int cellWidth, int cellHeight)
{
	float x = (celdaActual.row_ * cellWidth) +  (0.5f * cellWidth);
	float y = (celdaActual.col_ * cellHeight) + (cellHeight * 0.5f);
	float a = pow(abs(x - (*objetoActual)->position.x),2);
	float b = pow(abs(y - (*objetoActual)->position.y),2);
    	return (sqrt(a+b));   	
}

float distancia(Celda celdaActual,std::list<Object*>::iterator objetoActual, int cellWidth, int cellHeight)
{
	float x = (celdaActual.row_ * cellWidth) +  (0.5f * cellWidth);
	float y = (celdaActual.col_ * cellHeight) + (cellHeight * 0.5f);
	float a = pow(abs(x - (*objetoActual)->position.x),2);
	float b = pow(abs(y - (*objetoActual)->position.y),2);
    	return (sqrt(a+b));   	
}
 

float defaultCellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
    , float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
    	
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    Vector3 cellPosition((col * cellWidth) + cellWidth * 0.5f, (row * cellHeight) + cellHeight * 0.5f, 0);
    	
    float val = 0;
    for (List<Object*>::iterator it=obstacles.begin(); it != obstacles.end(); ++it) {
	    val += _distance(cellPosition, (*it)->position);
    }

    return val;
}


bool factible(Celda celdaActual, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses, std::list<Defense*>::iterator defensa_actual)
{
	/*  Una posición es factible si:
 	       -no pisa ninguna defensa
  	       -no pise ningun obstaculo
	       -no se salga del mapa
  	*/
	bool es_factible = true;
	float cellWidth = mapWidth / nCellsWidth;
    	float cellHeight = mapHeight / nCellsHeight; 
    	float centro_x,centro_y; 
    	
	//No pisa ninguna defensa
	for(auto i = defenses.begin(); i != defenses.end(); i++)
	{
		//si la suma de los radios es mayor que la distacia que separa entre los centros de las mismas
		float sumaRadios = (*i)->radio + (*defensa_actual)->radio;
		if(distancia(celdaActual,i,cellWidth,cellHeight) < sumaRadios) es_factible = false;
	}
	
	//No pisa ningún objeto
	for(auto i = obstacles.begin(); i != obstacles.end(); i++)
	{
		//si la suma de los radios es mayor que la distacia que separa entre los centros de las mismas
		float sumaRadios = (*i)->radio + (*defensa_actual)->radio;
		if(distancia(celdaActual,i,cellWidth,cellHeight) < sumaRadios) es_factible = false;
	}
	
	//No sale del mapa
	centro_x = (celdaActual.row_ * cellWidth) +  (0.5f * cellWidth);
	centro_y = (celdaActual.col_ * cellHeight) + (cellHeight * 0.5f);
	
	if((centro_x + (*defensa_actual)->radio) >= mapWidth) es_factible = false;
	else if((centro_y + (*defensa_actual)->radio) >= mapHeight) es_factible = false;
	else if((centro_x - (*defensa_actual)->radio) < 0) es_factible = false;
	else if((centro_y - (*defensa_actual)->radio) < 0) es_factible = false;
	
	return es_factible;
}





/*	Algoritmos de Ordenación	*/

void fusion(std::vector<Celda>& v, int i, int k, int j)
{
	int n = j - i + 1;
	int p = i;
	int q = k;
	std::vector<Celda> w;
	
	for(int l = 0; l < n; l++)
	{
		if(p < k && (q > j-1 || v[p] <= v[q]))
		{
			w.push_back(v[p]);
			p = p + 1;
		}
		else
		{
			w.push_back(v[q]);
			q = q + 1;
		}	
	}
	
	for(int l = 0; l < n; l++)
	{
		v[i + l] = w[l];
	}
}


void ordenacion_insercion(std::vector<Celda>& v, int i, int j)
{
	int k;
	Celda aux;
	
	for(int t = i; t <= j; t++)	
	{

		aux = v[t];
		for(k = t; k > 0 && (aux < v[k-1]); k--)
		{
			v[k] = v[k-1];
		}
		v[k] = aux;
	}
}


void ordenacion_fusion(std::vector<Celda>& v, int i, int j)
{
	int n = j - i + 1;
	int k = 0;
	
	if(n <= 3)
		ordenacion_insercion(v,i,j);
	else
	{	
		k = i - 1 + n/2;
		ordenacion_fusion(v,i,k);
		ordenacion_fusion(v, k+1, j);
		fusion(v,i,k,j);
	} 
}


int pivote(std::vector<Celda>& v, int i, int j)
{
	int p = i;
	Celda x = v[i];
	Celda aux;
	for(int k=i+1;k < j;k++)
	{
		if(v[k].value_ <= x.value_)
		{
			p=p+1;
			aux = v[p];
			v[p] = v[k];
			v[k] = aux;
		}
	}
	
	v[i] = v[p];
	v[p] = x;
	
	return p;
}


void ordenacion_rapida(std::vector<Celda>& v, int i, int j)
{
	int p;
	int n = j - i + 1;
	
	if(n <= 3)
	{
		ordenacion_insercion(v,i,j);
	}
	else
	{
		p = pivote(v,i,j);
		ordenacion_rapida(v,i,p-1);
		ordenacion_rapida(v,p+1,j);
	}
}


void ordenacion_monticulo(std::vector<Celda>& v)
{
	std::make_heap(v.begin(),v.end());
	std::sort_heap(v.begin(),v.end());
}


void sin_ordenacion(std::vector<Celda>& v)
{
	float mayor = 0.0f;
	int posmayor = 0;
	for(int i = 0; i <= v.size()-1; i++)
	{
		if(v[i].value_ > mayor)
		{
			mayor = v[i].value_;
			posmayor = i;
		}
	}
	
	//std::cout<<"Mayor"<<&v[posmayor]<<"final"<<&v[v.size()-1]<<std::endl;	
	std::swap(v[posmayor],v[v.size()-1]);	
}


//Comprobamos que el vector esta ordenado de menor a mayor
bool comprobar_ordenado(const std::vector<Celda>& v)
{
	bool ordenado = true;
	
	for(int i = 1; i <= v.size() - 1; i++)
	{
		if(v[i-1].value_ > v[i].value_) 
			ordenado = false;
	}

	return ordenado;
}

//Pruebas de caja negra. Dado un vector de 6 Celdas, se comprobaran todas las permutaciones
//posibles y se ordenaran, para posteriormente comprobar que el vector esta ordenado.
void caja_negra()
{
	std::vector<Celda> v;
	std::vector<Celda> w;
	std::vector<Celda> v2;
	
	
	for(int i = 0; i != 6; i++)
	{
		v.push_back(Celda(0,0,i));
		w.push_back(Celda(0,0,i));	
	}	
  		
    	/*	ORDENACION POR FUSION	*/	
    	do{
    		v2 = v;
    		ordenacion_fusion(v2, 0, v2.size()-1);
		if(!comprobar_ordenado(v2))
		 	std::cout<<"ERROR: Con fusion no está ordenado"<<std::endl;   
    	
    	}while(std::next_permutation(v.begin(),v.end()));  	
       	
    	  	
	/*	ORDENACION RAPIDA	*/	
    	do{
    		v2 = w;
		ordenacion_rapida(v2, 0, v2.size()-1);    	
    		if(!comprobar_ordenado(v2))
    			std::cout<<"ERROR: Con ord.rapida no está ordenado"<<std::endl;    	
    	}while(std::next_permutation(w.begin(),w.end()));  	

}



/*	PLACE DEFENSES	*/


void DEF_LIB_EXPORTED placeDefenses3(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , List<Object*> obstacles, List<Defense*> defenses) {
/*
    cronometro c;
    c.activar();
*/
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    
    
    List<Defense*>::iterator currentDefense;
    std::vector<Celda> celdasCandidatas;
    std::vector<Celda> celdasCandidatas2;
    bool colocado;
    
    for(int i = 0; i < nCellsWidth; i++)
    {
    	for(int j = 0; j < nCellsHeight; j++)
    	{
    		celdasCandidatas.push_back(Celda(i,j,defaultCellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses)));
    	}
    }
    
    
    /*	------	PRUEBA DE CAJA NEGRA  -------	*/   
        
    //Descomentar para hacer prueba de caja negra
    /*
	caja_negra();
    */
    
    
    
    
    /*	------	ALGORITMOS DE ORDENACION  ------  */
    
    //Descomentar para calcular los tiempos de cada algoritmo de ordenacion
   
   
	    cronometro c1, c2, c3, c4;                
	    long int r1 = 0,r2 = 0,r3 = 0,r4 = 0;
	    const double e_abs = 0.01, // Máximo error absoluto cometido.
	    		e_rel = 0.001; // Máximo error relativo aceptado.

	    c1.activar();
	    do {	
		
		    colocado = false;    
		    //Ordenamos para obtener siempre los de mayor valor
		    celdasCandidatas2 = celdasCandidatas;	    

		    
		    currentDefense = defenses.begin();
		 
		    //Vamos colocando las defensas restantes y comprobando aquellas que sean factibles
		    while(currentDefense != defenses.end()) 
		    {
		    	colocado = false;
		    	
		    	while(!celdasCandidatas2.empty() && !colocado)	    	
		    	{
		    		sin_ordenacion(celdasCandidatas2);
		    		
			    	if(factible(celdasCandidatas2.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, currentDefense))
			    	{
			    		(*currentDefense)->position.x = (celdasCandidatas2.back().row_ * cellWidth) +  (0.5f * cellWidth);
		    			(*currentDefense)->position.y = (celdasCandidatas2.back().col_ * cellHeight) + (cellHeight * 0.5f);
			    		colocado = true;
			     	}
			    	celdasCandidatas2.pop_back();
		    	}    	
		    	currentDefense++;
		    }  	
			
		    ++r1;
		    
	    } while(c1.tiempo() <  e_abs / e_rel + e_abs);    
	    c1.parar();    
	    
	    
		
	    
	    
	    c2.activar();
	    do {	
		
		    colocado = false;    
		    //Ordenamos para obtener siempre los de mayor valor
		    celdasCandidatas2 = celdasCandidatas;
			    
		
		    ordenacion_fusion(celdasCandidatas2, 0, celdasCandidatas2.size()-1);

		    
		    currentDefense = defenses.begin();
		 
		    //Vamos colocando las defensas restantes y comprobando aquellas que sean factibles
		    while(currentDefense != defenses.end()) 
		    {
		    	colocado = false;
		    	
		    	while(!celdasCandidatas2.empty() && !colocado)
		    	{
			    	if(factible(celdasCandidatas2.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, currentDefense))
			    	{
			    		(*currentDefense)->position.x = (celdasCandidatas2.back().row_ * cellWidth) +  (0.5f * cellWidth);
		    			(*currentDefense)->position.y = (celdasCandidatas2.back().col_ * cellHeight) + (cellHeight * 0.5f);
			    		colocado = true;
			     	}
			    	celdasCandidatas2.pop_back();
		    	}    	
		    	currentDefense++;
		    }  	
			
		    ++r2;
		    
	    } while(c2.tiempo() <  e_abs / e_rel + e_abs);    
	    c2.parar(); 
	    
	    
	    
	    
	    c3.activar();
	    do {	
		
		    colocado = false;    
		    //Ordenamos para obtener siempre los de mayor valor
		    celdasCandidatas2 = celdasCandidatas;
		    
		    
		    ordenacion_rapida(celdasCandidatas2, 0, celdasCandidatas.size()-1);
		    currentDefense = defenses.begin();
		 
		    //Vamos colocando las defensas restantes y comprobando aquellas que sean factibles
		    while(currentDefense != defenses.end()) 
		    {
		    	colocado = false;
		    	
		    	while(!celdasCandidatas2.empty() && !colocado)
		    	{
			    	if(factible(celdasCandidatas2.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, currentDefense))
			    	{
			    		(*currentDefense)->position.x = (celdasCandidatas2.back().row_ * cellWidth) +  (0.5f * cellWidth);
		    			(*currentDefense)->position.y = (celdasCandidatas2.back().col_ * cellHeight) + (cellHeight * 0.5f);
			    		colocado = true;
			     	}
			    	celdasCandidatas2.pop_back();
		    	}    	
		    	currentDefense++;
		    }  	
			
		    ++r3;
		    
	    } while(c3.tiempo() <  e_abs / e_rel + e_abs);    
	    c3.parar(); 
	      
	      
	    
	    
	    c4.activar();
	    do {	
		
		    colocado = false;    
		    //Ordenamos para obtener siempre los de mayor valor
		    celdasCandidatas2 = celdasCandidatas;
		    
		
		    ordenacion_monticulo(celdasCandidatas2);
		    currentDefense = defenses.begin();
		 
		    //Vamos colocando las defensas restantes y comprobando aquellas que sean factibles
		    while(currentDefense != defenses.end()) 
		    {
		    	colocado = false;
		    	
		    	while(!celdasCandidatas2.empty() && !colocado)
		    	{
			    	if(factible(celdasCandidatas2.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, currentDefense))
			    	{
			    		(*currentDefense)->position.x = (celdasCandidatas2.back().row_ * cellWidth) +  (0.5f * cellWidth);
		    			(*currentDefense)->position.y = (celdasCandidatas2.back().col_ * cellHeight) + (cellHeight * 0.5f);
			    		colocado = true;
			     	}
			    	celdasCandidatas2.pop_back();
		    	}    	
		    	currentDefense++;
		    }  	
			
		    ++r4;
		    
	    } while(c4.tiempo() <  e_abs / e_rel + e_abs);    
	    c4.parar();   

	    
	    std::cout << (nCellsWidth * nCellsHeight) << '\t' << c1.tiempo() / r1 << '\t' << c2.tiempo() / r2 << '\t' << c3.tiempo() / r3 << '\t' << c4.tiempo() / r4 << std::endl;
	    
	    
    
    



	/*  ------  ESTRATEGIA SELECCIONADA  ------  */
	/*
	    
	    colocado = false;    
	    //Ordenamos para obtener siempre los de mayor valor
	    
	    
	    ordenacion_rapida(celdasCandidatas, 0, celdasCandidatas.size()-1);
	    currentDefense = defenses.begin();
	 
	    //Vamos colocando las defensas restantes y comprobando aquellas que sean factibles
	    while(currentDefense != defenses.end()) 
	    {
	    	colocado = false;
	    	
	    	while(!celdasCandidatas.empty() && !colocado)
	    	{
		    	if(factible(celdasCandidatas.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, currentDefense))
		    	{
		    		(*currentDefense)->position.x = (celdasCandidatas.back().row_ * cellWidth) +  (0.5f * cellWidth);
	    			(*currentDefense)->position.y = (celdasCandidatas.back().col_ * cellHeight) + (cellHeight * 0.5f);
		    		colocado = true;
		     	}
		    	celdasCandidatas.pop_back();
	    	}    	
	    	currentDefense++;
	    }    
*/
/*

	c.parar();

	std::cout<<c.tiempo()<<std::endl;

*/
}
