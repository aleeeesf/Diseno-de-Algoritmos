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
	/*	Una posición es factible si:
	  		-no pisa ninguna defensa
	  		-no pise ningun obstaculo
	  		-no se salga del mapa
  	*/
	bool es_factible = true;
	float cellWidth = mapWidth / nCellsWidth;
    	float cellHeight = mapHeight / nCellsHeight; 
    	
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
	float centro_x = (celdaActual.row() * cellWidth) +  (0.5f * cellWidth);
	float centro_y = (celdaActual.col() * cellHeight) + (cellHeight * 0.5f);
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
    	float x,y,distanciapuntos=0;
    	x=row*cellWidth+cellWidth*0.5f;
    	y=col*cellHeight+cellHeight*0.5f;  
  	float centro_cuadrante = 0, posicion = 0;
  	float valor=255;

	/* 	Cuadrante de las filas		*/	
	if(row <= nCellsHeight/4)
	{
		centro_cuadrante = (nCellsHeight/4)/2;
		posicion = abs(centro_cuadrante - row);
		valor -= posicion;
	}

	else if(row <= (nCellsHeight/4)*2)
	{
		centro_cuadrante = ((nCellsHeight/4)*2 - (nCellsHeight/4)/2);
		posicion = abs(centro_cuadrante - row);
		valor -= posicion;
	}

	else if(row <= (nCellsHeight/4)*3)
	{
		centro_cuadrante = ((nCellsHeight/4)*3 - (nCellsHeight/4)/2);
		posicion = abs(centro_cuadrante - row);
		valor -= posicion;
	}

	else if(row <= nCellsHeight)
	{
		centro_cuadrante = (nCellsHeight - (nCellsHeight/4)/2);
		posicion = abs(centro_cuadrante - row);
		valor -= posicion;
	}

	/*	Cuandrante de las columnas	*/	
	if(col <= nCellsWidth/4)
	{
		centro_cuadrante = (nCellsWidth/4)/2;
		posicion = abs(centro_cuadrante - col);
		valor -= posicion;

	}

	else if(col <= (nCellsWidth/4)*2)
	{
		centro_cuadrante = ((nCellsWidth/4)*2 - (nCellsWidth/4)/2);
		posicion = abs(centro_cuadrante - col);
		valor -= posicion;
	}

	else if(col <= (nCellsWidth/4)*3)
	{
		centro_cuadrante = ((nCellsWidth/4)*3 - (nCellsWidth/4)/2);
		posicion = abs(centro_cuadrante - col);
		valor -= posicion;
	}

	else if(col <= nCellsWidth)
	{
		centro_cuadrante = (nCellsWidth - (nCellsWidth/4)/2);
		posicion = abs(centro_cuadrante - col);
		valor -= posicion;
	}
	
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

	centro_cuadrante = (nCellsWidth/2);
	posicion = abs(centro_cuadrante - col);
	valor -= posicion;
	posicion = abs(centro_cuadrante - row);
	valor -= posicion;

	return valor;
}

/*
float cellValue3(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	
	float cellWidth = mapWidth / nCellsWidth;
    	float cellHeight = mapHeight / nCellsHeight; 
    	float x,y,distanciapuntos=0;
    	x=row*cellWidth+cellWidth*0.5f;
    	y=col*cellHeight+cellHeight*0.5f;  
  	float centro_cuadrante = 0, posicion = 0;
  	float valor=0,valor2=0;  	
  	
  	if( (mapWidth - (row*cellWidth)) < (5 * cellWidth)) 	valor += (mapWidth - (row*cellWidth));	
  	if( (mapHeight - (col * cellHeight)) < (5 * cellHeight)) 	valor += (mapHeight - (col * cellHeight)); 	

	valor+=row+col;
	
	return valor;
}

float cellValue_defensas(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	
	float cellWidth = mapWidth / nCellsWidth;
    	float cellHeight = mapHeight / nCellsHeight; 
    	float x,y,distanciapuntos=0;
    	x=row*cellWidth+cellWidth*0.5f;
    	y=col*cellHeight+cellHeight*0.5f;  
  	float centro_cuadrante = 0, posicion = 0;
  	float valor=255, valor2=0;
	List<Defense*>::iterator currentDefense = defenses.begin();

	valor2 = abs((*currentDefense)->position.x - row*cellWidth);
	valor -= valor2;
	valor2 = abs((*currentDefense)->position.y - col*cellHeight);
	valor -= valor2;

	if(valor2 == abs((*currentDefense)->position.x - row*cellWidth)) valor += valor2;
	
	return valor;
}

*/

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 
    bool colocado = false;

    List<Defense*>::iterator currentDefense = defenses.begin();
    std::vector<Celda> celdasCandidatas;
    
    for(int i = 0; i < nCellsWidth; i++)
    {
    	for(int j = 0; j < nCellsHeight; j++)
    	{
    		celdasCandidatas.push_back(Celda(i,j,cellValue2(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses)));
    	}
    }
    
    sort(celdasCandidatas.begin(),celdasCandidatas.end());    
    
    while(!celdasCandidatas.empty() && !colocado)
    {
    	if(factible(celdasCandidatas.back(), freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses, defenses.begin()))
    	{
    		(*currentDefense)->position.x = (celdasCandidatas.back().row() * cellWidth) +  (0.5f * cellWidth);
    		(*currentDefense)->position.y = (celdasCandidatas.back().col() * cellHeight) + (cellHeight * 0.5f);
    		colocado = true;
    	}    	
    	celdasCandidatas.pop_back();
    }
    
    currentDefense++;
    celdasCandidatas.clear();
    
    for(int i = 0; i < nCellsWidth; i++)
    {
    	for(int j = 0; j < nCellsHeight; j++)
    	{
    		celdasCandidatas.push_back(Celda(i,j,cellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses)));
    	}
    }
    
    sort(celdasCandidatas.begin(),celdasCandidatas.end()); 
 
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
           cellValues[i][j] = ((int)(cellValue(i, j, freeCells, nCellsWidth, nCellsHeight, mapWidth, mapHeight, obstacles, defenses))) % 256;
         }
    }
    	/*
	for(int i = 0; i < nCellsHeight; ++i) {

       		for(int j = 0; j < nCellsWidth; ++j) {

          	 std::cout<<cellValues[i][j]<<" ";
         	}
         	std::cout<<std::endl;
    	}
    */
    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells
                         , cellValues, std::list<Defense*>(), true);

    for(int i = 0; i < nCellsHeight ; ++i)
        delete [] cellValues[i];
	delete [] cellValues;
	cellValues = NULL;

#endif
}
