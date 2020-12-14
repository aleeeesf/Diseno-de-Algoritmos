#include <iostream>
#include <vector>
#include <algorithm>

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


void ordenacion_monticulo(std::vector<int>& v)
{
	std::make_heap(v.begin(),v.end());
	std::sort_heap(v.begin(),v.end());
}

struct Celda
{

		int row_, col_;
		float value_;
		Celda():row_(0),col_(0),value_(0.0f){}
		Celda(int fila, int columna, float valor):row_(fila),col_(columna),value_(valor){}
};

bool operator < (const Celda& a, const Celda& b){ return a.value_ < b.value_;}
bool operator <= (const Celda& a, const Celda& b){ return a.value_ <= b.value_;}


void sin_ordenacion(std::vector<Celda>& v)
{

	float mayor = 0.0f;
	int posmayor;
	for(int i = 0; i != v.size(); i++)
	{
		if(v[i].value_ > mayor)
		{
			mayor = v[i].value_;
			posmayor = i;
		}
	}
	
	posmayor;
	
	std::swap(v[posmayor],v.back());

}












int main()
{

	std::vector<Celda> v;
	Celda a(1,1,7.0f);
	Celda b(2,2,4.0f);

	
	v.push_back(a);
	v.push_back(b);


	
	

	
	sin_ordenacion(v);
	
	
	for(std::vector<Celda>::iterator i = v.begin(); i != v.end(); i++)
	{
		std::cout<<(*i).value_<<std::endl;
	}
	
	
	return 0;
}
