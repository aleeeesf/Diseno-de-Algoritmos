// ###### Config options ################

#define PRINT_PATHS 1

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_PATHS
#include "ppm.h"
#endif

using namespace Asedio;


bool es_menor(const AStarNode* a,const AStarNode* b)
{
	return (a->F > b->F);
}

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ 
    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); 
}

void DEF_LIB_EXPORTED calculateAdditionalCost(float** additionalCost
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;
    int celda_x, celda_y;
    float valor,posicion;
    
    List<Defense*>::iterator currentDefense = defenses.begin();


    for(int i = 0 ; i < cellsHeight ; ++i) {
        for(int j = 0 ; j < cellsWidth ; ++j) {            
           
            Vector3 cellPosition = cellCenterToPosition(i, j, cellWidth, cellHeight);
            float cost = 0; 
            auto it = defenses.begin();
            it++;
            
            //Asignamos un mayor valor a aquellas posiciones que más cerca estén de las defensas
            for(it; it != defenses.end(); it++)
	    {
		celda_x = (((*it)->position.x - cellWidth/2) / cellWidth);
		celda_y = (((*it)->position.y - cellHeight/2) / cellHeight);

		posicion = abs(celda_x - i);
		valor += posicion;
		posicion = abs(celda_y - j);
		valor += posicion;
		cost = valor * 100;
	    }
             
            additionalCost[i][j] = cost;
        }
    }    
}

/*

void DEF_LIB_EXPORTED calculateAdditionalCost(float** additionalCost
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / cellsWidth;
    float cellHeight = mapHeight / cellsHeight;

    for(int i = 0 ; i < cellsHeight ; ++i) {
        for(int j = 0 ; j < cellsWidth ; ++j) {
            Vector3 cellPosition = cellCenterToPosition(i, j, cellWidth, cellHeight);
            float cost = 0;
            if( (i+j) % 2 == 0 ) {
                cost = cellWidth * 100;
            }
            
            additionalCost[i][j] = cost;
        }
    }
}
*/

void DEF_LIB_EXPORTED calculatePath(AStarNode* originNode, AStarNode* targetNode
                   , int cellsWidth, int cellsHeight, float mapWidth, float mapHeight
                   , float** additionalCost, std::list<Vector3> &path) {

/*
    int maxIter = 100;
    AStarNode* current = originNode;
    while(current != targetNode && maxIter > 0) { // @todo ensure current and target are connected
	    float min = INF_F;
	    AStarNode* o = NULL;    
	    for (List<AStarNode*>::iterator it=current->adjacents.begin(); it != current->adjacents.end(); ++it) {
		    float dist = _sdistance((*it)->position, targetNode->position);
            if(additionalCost != NULL) { 
                dist += additionalCost[(int)((*it)->position.y / cellsHeight)][(int)((*it)->position.x / cellsWidth)];
            }
		    //std::cout << (*it)->position.y << ", " << (*it)->position.x << std::endl;
		    if(dist < min) {
			    min = dist;
			    o = (*it);
		    }
	    }

	    current = o;

        if(current == NULL) {
            break;
        }

        path.push_back(current->position);
        --maxIter;
    }
    
    */
    
    //Lista de abiertos y cerrados
    std::vector<AStarNode*> abiertos,cerrado;
    AStarNode* current = originNode;
    bool encontrado = false;
    float d = 0.0f;
    
    targetNode->parent = NULL;
    current->G = 0;
    current->H = _sdistance(originNode->position, targetNode->position);
    current->F = originNode->G + originNode->H + additionalCost[(int)(originNode->position.y / cellsHeight)][(int)(originNode->position.x / cellsWidth)]; 
    
    abiertos.push_back(current);
    std::make_heap(abiertos.begin(),abiertos.end(),es_menor);   
    
    
    while(!encontrado && !abiertos.empty())
    {
    	current = abiertos.front();        	
    	std::pop_heap(abiertos.begin(),abiertos.end(),es_menor);
    	abiertos.pop_back();    	
    	
    	cerrado.push_back(current);
    	
    	if(current == targetNode)
    		encontrado = true;
    	
    	else
    	{
    		for(List<AStarNode*>::iterator j=current->adjacents.begin(); j != current->adjacents.end(); ++j)
    		{
    			std::vector<AStarNode*>::iterator it = std::find(cerrado.begin(),cerrado.end(),(*j));
    			
    			if(it == cerrado.end())
    			{
    				it = std::find(abiertos.begin(),abiertos.end(),(*j));
    				
    				if(it == abiertos.end())
    				{
    					(*j)->parent = current;
    					(*j)->G = current->G + _sdistance(current->position, (*j)->position);
    					(*j)->H = _sdistance((*j)->position, targetNode->position); 
    					(*j)->F = (*j)->G + (*j)->H + additionalCost[(int)((*j)->position.y / cellsHeight)][(int)((*j)->position.x / cellsWidth)];  
    					abiertos.push_back((*j)); 	
    					std::push_heap(abiertos.begin(),abiertos.end(),es_menor);			
    				}
    				else
    				{
    					d = _sdistance(current->position,(*j)->position);
    					
    					if((*j)->G > (current->G + d))
    					{
    						(*j)->parent = current;
    						(*j)->G = current->G + d;
    						(*j)->F = (*j)->G + (*j)->H + additionalCost[(int)((*j)->position.y / cellsHeight)][(int)((*j)->position.x / cellsWidth)];   	
    						std::make_heap(abiertos.begin(),abiertos.end(),es_menor);					   					
    					}    				
    				}
    			}  		
    		}    	
    	}    
    }
    
    //Recueración del camino
    current = targetNode;    
    path.push_front(targetNode->position);
    
    while(current->parent != originNode && targetNode->parent != NULL){
        current = current->parent;
        path.push_front(current->position);
    }

}
