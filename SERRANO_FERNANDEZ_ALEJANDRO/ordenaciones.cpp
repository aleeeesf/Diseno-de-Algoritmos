#include <iostream>
#include <vector>


void fusion(std::vector<int>& v, int i, int k, int j)
{
	int n = j - i + 1;
	int p = i;
	int q = k +1;
	std::vector<int> w;
	
	for(int l = 0; l < n; l++)
	{
		if(p <= k && (q > j || v[p] <= v[q]))
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
		v[i - 1 + l] = w[l];
	}
}

void ordenacion_insercion(std::vector<int>& v, int i, int j)
{
	int k;
	int aux;
	
	for(int i = 0; i < j; i++)
	{
		aux = v[i];
		for(k = i; k > 0 && (aux < v[k-1]); k++)
		{
			v[k] = v[k-1];
		}
		v[k] = aux;
	}
}


void ordenacion_fusion(std::vector<int>& v, int i, int j)
{
	int n = j - i + 1;
	int k = 0;
	
	if(n < 3)
		ordenacion_insercion(v,i,j);
	else
	{
		k = i -1 + n/2;
		ordenacion_fusion(v,i,k);
		ordenacion_fusion(v, k+1, j);
		fusion(v,i,k,j);
	} 
}



void ordenacion_rapida()
{


}






int main()
{

	std::vector<int> v;
	
	v.push_back(4);
	v.push_back(3);
	v.push_back(2);
	v.push_back(1);
	v.push_back(0);
	
	ordenacion_fusion(v,0,4);
	
	for(std::vector<int>::iterator i = v.begin(); i != v.end(); i++)
	{
		std::cout<<*i<<std::endl;
	}
	
	
	return 0;
}
