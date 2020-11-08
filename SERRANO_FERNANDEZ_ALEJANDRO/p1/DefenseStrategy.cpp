/*Arreglar factible y distancia*/




// ###### Config options ################

#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include <math.h>

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

using namespace Asedio;

class Celda
{
	private:
		int row_, col_;
		float value_;
	public:
		Celda(int fila, int columna, float valor):row_(fila),col_(columna),value_(valor){};
		const int row() const {return row_;}
		const int col() const {return col_;}
		const int valor() const {return value_;}
};

bool operator < (const Celda& a, const Celda& b){ return a.valor() < b.valor();}

float distancia(Celda celdaActual,std::list<Defense*>::iterator objetoActual, int cellWidth, int cellHeight)
{
	float x = (celdaActual.row() * cellWidth) +  (0.5f * cellWidth);
	float y = (celdaActual.col() * cellHeight) + (cellHeight * 0.5f);
	float a = pow(abs(x - (*objetoActual)->position.x),2);
	float b = pow(abs(y - (*objetoActual)->position.y),2);
    	return (sqrt(a+b));   	
}

float distancia(Celda celdaActual,std::list<Object*>::iterator objetoActual, int cellWidth, int cellHeight)
{
	float x = (celdaActual.row() * cellWidth) +  (0.5f * cellWidth);
	float y = (celdaActual.col() * cellHeight) + (cellHeight * 0.5f);
	float a = pow(abs(x - (*objetoActual)->position.x),2);
	float b = pow(abs(y - (*objetoActual)->position.y),2);
    	return (sqrt(a+b));   	
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
	centro_x = (celdaActual.row() * cellWidth) +  (0.5f * cellWidth);
	centro_y = (celdaActual.col() * cellHeight) + (cellHeight * 0.5f);
	
	if((centro_x + (*defensa_actual)->radio) >= mapWidth) es_factible = false;
	else if((centro_y + (*defensa_actual)->radio) >= mapHeight) es_factible = false;
	else if((centro_x - (*defensa_actual)->radio) < 0) es_factible = false;
	else if((centro_y - (*defensa_actual)->radio) < 0) es_factible = false;
	
	return es_factible;
}


float cellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	
	float cellWidth = mapWidth / nCellsWidth;
    	float cellHeight = mapHeight / nCellsHeight; 
    	float x,y,celda_x_objeto,celda_y_objeto;
    	x=row*cellWidth+cellWidth*0.5f;
    	y=col*cellHeight+cellHeight*0.5f;  
  	float centro_cuadrante = 0, posicion = 0;
  	float valor=255, max_radio = 0, max_valor = 0;
	int celda_x, celda_y;
	float porcentaje = 0.40f;
	
	bool borde = false;
	List<Object*>::iterator obstaculoGanador,obstaculoGanador_valor, obstaculoGanador_radio;
	
	for(List<Object*>::iterator i = obstacles.begin(); i != obstacles.end(); i++)
	{
		posicion = 0; valor = 255;
		celda_x = (nCellsWidth/2);
		celda_y = (nCellsHeight/2);
		celda_x_objeto = (int)(((*i)->position.x - cellWidth/2) / cellWidth);
		celda_y_objeto = (int)(((*i)->position.y - cellHeight/2) / cellHeight);	

		posicion = abs(celda_x - celda_x_objeto);
		valor -= posicion;
		posicion = abs(celda_y - celda_y_objeto);
		valor -= posicion;
		
		if((*i)->radio > max_radio)
		{
			max_radio = (*i)->radio;
			obstaculoGanador_radio = i;
		}
		
		if(valor > max_valor)
		{
			obstaculoGanador_valor = i;
			max_valor = valor;
		}
	}
	
	celda_x_objeto = (((*obstaculoGanador_radio)->position.x - cellWidth/2) / cellWidth);
	celda_y_objeto = (((*obstaculoGanador_radio)->position.y - cellHeight/2) / cellHeight);	
	

	
	if(abs(mapWidth - celda_x_objeto*cellWidth) < (porcentaje*mapWidth)) borde = true;
	else if(abs(mapHeight - celda_y_objeto*cellHeight) < (porcentaje*mapHeight)) borde = true;
	else if(abs(0 - celda_x_objeto*cellWidth) < (porcentaje*mapWidth)) borde = true;
	else if(abs(0 - celda_y_objeto*cellHeight) < (porcentaje*mapHeight)) borde = true;
	
	
	if(borde) obstaculoGanador = obstaculoGanador_valor;
	else obstaculoGanador = obstaculoGanador_radio;

	posicion = 0; valor = 255;
	celda_x = (((*obstaculoGanador)->position.x - cellWidth/2) / cellWidth);
	celda_y = (((*obstaculoGanador)->position.y - cellHeight/2) / cellHeight);

	posicion = abs(celda_x - row);
	valor -= posicion;
	posicion = abs(celda_y - col);
	valor -= posicion;

	return valor;
}

float cellValue2(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	
	float cellWidth = mapWidth / nCellsWidth;
    	float cellHeight = mapHeight / nCellsHeight; 
    	float x,y,distanciapuntos=0;
    	x=row*cellWidth+cellWidth*0.5f;
    	y=col*cellHeight+cellHeight*0.5f;  
  	float centro_cuadrante = 0, posicion = 0;
  	float valor=255;
	int celda_x, celda_y;
	
	List<Defense*>::iterator defensaPrincipal = defenses.begin();
	
	//Se colocan alrededor de la defensa principal
	//Las celdas mas cercana al la defensa tendrán mayor valor
	celda_x = (((*defensaPrincipal)->position.x - cellWidth/2) / cellWidth);
	celda_y = (((*defensaPrincipal)->position.y - cellHeight/2) / cellHeight);

	posicion = abs(celda_x - row);
	valor -= posicion;
	posicion = abs(celda_y - col);
	valor -= posicion;

	return valor;
}


void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 
    bool colocado = false;

    List<Defense*>::iterator currentDefense = defenses.begin();
    std::vector<Celda> celdasCandidatas;
    
    //Introducimos todas las celdas con sus correspondientes valores
    for(int i = 0; i < nCellsWidth; i++)
    {
    	for(int j = 0; j < nCellsHeight; j++)
    	{
    		celdasCandidatas.push_back(Celda(i,j,cellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses)));
    	}
    }
    
    sort(celdasCandidatas.begin(),celdasCandidatas.end());    
    
    //Vamos sacando todas las celdas introducidas anteriormente (Aquellas que tengan mayor valor). 
    while(!celdasCandidatas.empty() && !colocado)
    {
    	//Si es factible colocamos el centro de extracción
    	if(factible(celdasCandidatas.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, defenses.begin()))
    	{
    		(*currentDefense)->position.x = (celdasCandidatas.back().row() * cellWidth) +  (0.5f * cellWidth);
    		(*currentDefense)->position.y = (celdasCandidatas.back().col() * cellHeight) + (cellHeight * 0.5f);
    		colocado = true;
    	}    	
    	celdasCandidatas.pop_back();
    }
    
    currentDefense++;
    
    //Volvemos a intrducir todas las celdas, pero esta vez con otro cellValue
    celdasCandidatas.clear();
    
    for(int i = 0; i < nCellsWidth; i++)
    {
    	for(int j = 0; j < nCellsHeight; j++)
    	{
    		celdasCandidatas.push_back(Celda(i,j,cellValue2(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses)));
    	}
    }
    
    //Ordenamos para obtener siempre los de mayor valor
    sort(celdasCandidatas.begin(),celdasCandidatas.end()); 
 
    //Vamos colocando las defensas restantes y comprobando aquellas que sean factibles
    while(currentDefense != defenses.end()) 
    {
    	colocado = false;
    	
    	while(!celdasCandidatas.empty() && !colocado)
    	{
	    	if(factible(celdasCandidatas.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, currentDefense))
	    	{
	    		(*currentDefense)->position.x = (celdasCandidatas.back().row() * cellWidth) +  (0.5f * cellWidth);
    			(*currentDefense)->position.y = (celdasCandidatas.back().col() * cellHeight) + (cellHeight * 0.5f);
	    		colocado = true;
	     	}
	    	celdasCandidatas.pop_back();
    	}    	
    	currentDefense++;
    }  

    
#ifdef PRINT_DEFENSE_STRATEGY

    float** cellValues = new float* [nCellsHeight]; 
    for(int i = 0; i < nCellsHeight; ++i) {
       cellValues[i] = new float[nCellsWidth];
       for(int j = 0; j < nCellsWidth; ++j) {
           cellValues[i][j] = ((int)(cellValue2(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses))) % 256;
         }
    }

    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells
                         , cellValues, std::list<Defense*>(), true);

    for(int i = 0; i < nCellsHeight ; ++i)
        delete [] cellValues[i];
	delete [] cellValues;
	cellValues = NULL;

#endif
}
