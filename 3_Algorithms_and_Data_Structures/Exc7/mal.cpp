#include <iostream>
#include <vector>

using namespace std;

int a, b;
char c;

typedef struct elem
{
	short color;
	int km;
} elem;

int get_tree_size(int n)
{
	int pow = 1;
	while ( pow/2 < n )
		pow *= 2;
	
	return pow;
}

int ile_km( vector<elem> &highway, int i, int left, int right )
{
	int res = 0;
	if ( highway[i].color != 0 )
	{
		if ( c == 'B' && highway[i].color == 1 )
		{
			return highway[i].km;
		}
		if ( c == 'C' && highway[i].color == -1 )
		{
			return highway[i].km;
		}
		if ( c == 'B' ) // color == -1
		{
			if ( i >= highway.size()/2 )
			{
				highway[i].color = 1;
				highway[i].km = 1;
				return 1;
			}
			if ( a <= left && b >= right )
			{
				highway[i].color = 1;
				highway[i].km = right - left + 1;
				return highway[i].km;
			}
			else
			{
				highway[i*2].color = -1;
				highway[i*2 + 1].color = -1;
				highway[i*2].km = 0;
				highway[i*2 + 1].km = 0;
				
				if ( a <= (left + right)/2 )
				{
					res += ile_km(highway, i*2, left, (left+right)/2);
				}
				if ( b > (left + right)/2 )
				{
					res += ile_km(highway, i*2 + 1, (left+right)/2 + 1, right);
				}

				highway[i].km = highway[i*2].km + highway[i*2 + 1].km;
				highway[i].color = 0;
				
				return highway[i].km;
			}
		}
		else // c == 'C' && color == 1
		{
			if ( i >= highway.size()/2 )
			{
				highway[i].color = -1;
				highway[i].km = 0;
				return 0;
			}
			if ( a <= left && b >= right )
			{
				highway[i].color = -1;
				highway[i].km = 0;
				return 0;
			}
			else
			{
				highway[i*2].color = 1;
				highway[i*2 + 1].color = 1;
				highway[i*2].km = (left+right)/2 - left + 1;
				highway[i*2 + 1].km = right - (left+right)/2;
				
				if ( a <= (left + right)/2 )
				{
					res += ile_km(highway, i*2, left, (left+right)/2);
				}
				
				if ( b > (left + right)/2 )
				{
					res += ile_km(highway, i*2 + 1, (left+right)/2 + 1, right);
				}
				
				highway[i].km = highway[i*2].km+ highway[i*2 + 1].km;
				highway[i].color = 0;
				
				return highway[i].km;
			}
		}
	}
	else
	{
		if ( a <= left && b >= right )
		{
			if ( c == 'B' )
			{
				highway[i].km = right - left + 1;
				highway[i].color = 1;
				return highway[i].km;
			}
			if ( c == 'C' )
			{
				highway[i].km = 0;
				highway[i].color = -1;
				return 0;
			}
		}
		
			if ( a <= (left + right)/2 )
			{
				res += ile_km(highway, i*2, left, (left+right)/2);
			}
			if ( b > (left + right)/2 )
			{
				res += ile_km(highway, i*2 + 1, (left+right)/2 + 1, right);
			}
			
			highway[i].km = highway[i*2].km+ highway[i*2 + 1].km;

			if ( highway[i].km == 0 )
			{
				highway[i].color = -1;
			}
			else if ( highway[i].km == right - left + 1 )
			{
				highway[i].color = 1;
			}
		
		return highway[i].km;
	}
}

int main(int argc, char *argv[])
{	
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	
	int n, m;
	cin >> n;
	cin >> m;
	int km = 0, check = 0;
	
	int tree_size = get_tree_size(n);
	vector<elem> highway( tree_size, (elem){-1,0} );
	
	for ( int i = 0; i < m; i++ )
		{
			cin >> a;
			cin >> b;
			cin >> c;
			cout << ile_km( highway, 1, 1, tree_size/2 ) << endl;
		}
}

/*
5 12 C
-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 -1 -1 -1 

2 8 B
-1 1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 -1 -1 -1 -1 

4 7 B
:::::4 4
13 16 C
:::::4 4
1 10 C
:::::0 0
8 12 B
:::::5 5
8 12 B
:::::5 5
3 11 C
:::::-2 1
-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 1 -1 -1 -1 -1 
*/