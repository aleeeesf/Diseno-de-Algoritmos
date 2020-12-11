#include <iostream>
#include <vector>


void fusion(std::vector<int>& v, int i, int k, int j)
{
	int n = j - i;
	int p = i;
	int q = k;
	std::vector<int> w;
	
	for(int l = 0; l < n; l++)
	{
		if(p <= k && (q > j-1 || v[p] <= v[q]))
		{
			w.push_back(v[p]);
			p = p + 1;
		}
		else
		{
			w.push_back(v[p]);
			q = q + 1;
		}	
	}
	
	for(int l = 0; l < n; l++)
	{
		v[i + l] = w[l];
	}
}

void ordenacion_insercion(std::vector<int>& v, int i, int j)
{
	int k;
	int aux;
	
	for(int i = 0; i < j; i++)	
	{

		aux = v[i];
		for(k = i; k > 0 && (aux < v[k-1]); k--)
		{
			v[k] = v[k-1];
		}
		v[k] = aux;
	}
}


void ordenacion_fusion(std::vector<int>& v, int i, int j)
{
	int n = j - i;
	int k = 0;
	
	if(n < 3)
		ordenacion_insercion(v,i,j);
	else
	{
		k = j - ((j-1)/2);
		ordenacion_fusion(v,i,k);
		ordenacion_fusion(v, k, j);
		fusion(v,i,k,j);
	} 
}


int pivote(std::vector<int>& v, int i, int j)
{
	int p=i;
	int x=v[i];
	int aux;
	for(int k=i+1;k < j;k++)
	{
		if(v[k]<=x)
		{
			p=p+1;
			aux=v[p];
			v[p]=v[k];
			v[k]=aux;
		}
	}
	
	v[i]=v[p];
	v[p]=x;
	
	return p;
}


void ordenacion_rapida(std::vector<int>& v, int i, int j)
{
	int p;
	int n=j-i;
	
	if(n<3)
	{
		ordenacion_insercion(v,i,j);
	}
	else
	{
		p=pivote(v,i,j);
		ordenacion_rapida(v,i,p-1);
		ordenacion_rapida(v,p+1,j);
	}
}




int main()
{

	std::vector<int> v;
	
	
	v.push_back(7);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(1);
	v.push_back(0);
	
	ordenacion_rapida(v,0,v.size());
	
	for(std::vector<int>::iterator i = v.begin(); i != v.end(); i++)
	{
		std::cout<<*i<<std::endl;
	}
	
	
	return 0;
}
