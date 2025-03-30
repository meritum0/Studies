#include <iostream>
#include <vector>

using namespace std;

typedef struct element
{
	int64_t sum;
	int idx_niep_lewo;
	int idx_niep_prawo;
	int idx_parz_lewo;
	int idx_parz_prawo;
} element;

vector<element> preproces( vector<int> &ceny )
{
	vector<element> v(ceny.size());
	
	int64_t suma = 0;
	int idx_ostatni_niep = -1;
	int idx_ostatni_parz = -1;
	
	for ( int i = ceny.size() - 1; i >= 0; i-- )
	{
		element e = {0, -1, -1, -1, -1};
		if ( idx_ostatni_niep >= 0 )
			e.idx_niep_prawo = idx_ostatni_niep;
		if ( idx_ostatni_parz >= 0 )
			e.idx_parz_prawo = idx_ostatni_parz;
		if ( ceny[i] % 2 == 0 )
			idx_ostatni_parz = i;
		else 
			idx_ostatni_niep = i;
		
		suma += ceny[i];
		e.sum = suma;
		v[i] = e;
	}
	
	idx_ostatni_niep = -1;
	idx_ostatni_parz = -1;

	for ( int i = 0; i < ceny.size(); i++ )
	{
		if ( idx_ostatni_niep >= 0 )
			v[i].idx_niep_lewo = idx_ostatni_niep;
		if ( idx_ostatni_parz >= 0 )
			v[i].idx_parz_lewo = idx_ostatni_parz;
		if ( ceny[i] % 2 == 0 )
			idx_ostatni_parz = i;
		else
			idx_ostatni_niep = i;
	}
	
	return v;
}

int64_t findBest( int liczba_produktow, vector<int> &ceny, vector<element> &sufix )
{
	if ( liczba_produktow == 0 )
		return 0;
	
	int64_t suma = sufix[sufix.size() - liczba_produktow].sum;
	if ( suma % 2 == 1 )
		return suma;
	
	int zamiennik;
	int64_t suma_pierwsza = 0;
	int64_t suma_druga = 0;
	if ( ceny[ceny.size() - liczba_produktow] % 2 == 1 )
	{
		if ( sufix[sufix.size() - liczba_produktow].idx_parz_lewo >= 0 )
		{
			zamiennik = ceny[ sufix[sufix.size() - liczba_produktow].idx_parz_lewo ];
			suma_pierwsza = suma - ceny[ ceny.size() - liczba_produktow ] + zamiennik;
		}
		
		if ( sufix[sufix.size() - liczba_produktow].idx_niep_lewo >= 0 && sufix[sufix.size() - liczba_produktow].idx_parz_prawo >= 0 )
		{
			zamiennik = ceny[ sufix[sufix.size() - liczba_produktow].idx_niep_lewo ];
			suma_druga = suma - ceny[ sufix[sufix.size() - liczba_produktow].idx_parz_prawo ] + zamiennik;
		}
		suma = max( suma_pierwsza, suma_druga );
	}
	else
	{
		if ( sufix[sufix.size() - liczba_produktow].idx_niep_lewo >= 0 )
		{
			zamiennik = ceny[ sufix[sufix.size() - liczba_produktow].idx_niep_lewo ];
			suma_pierwsza = suma - ceny[ ceny.size() - liczba_produktow ] + zamiennik;
		}
		
		if ( sufix[sufix.size() - liczba_produktow].idx_parz_lewo >= 0 && sufix[sufix.size() - liczba_produktow].idx_niep_prawo >= 0 )
		{
			zamiennik = ceny[ sufix[sufix.size() - liczba_produktow].idx_parz_lewo ];
			suma_druga = suma - ceny[ sufix[sufix.size() - liczba_produktow].idx_niep_prawo] + zamiennik;
		}
		suma = max( suma_pierwsza, suma_druga );
	}

	if ( suma % 2 == 0 )
		return -1;
		
	return suma;
}

int main(int argc, char *argv[]) 
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
	int n, m;
	cin >> n;						// ilosc produktow na bazarku
	vector<int> ceny(n);
		
	for ( int i = 0; i < n; i++ ) 
		cin >> ceny[i];				// cena danego produktu
	
	cin >> m;						// ile dni
	vector<int> ile_produktow(m);
	
	for ( int i = 0; i < m; i++ )
		cin >> ile_produktow[i];		// ile produktow babcia chce kupic danego dnia
	
	auto sufix = preproces(ceny);
	
	for ( int i = 0; i < m; i++ )
		cout << findBest(ile_produktow[i], ceny, sufix) << "\n";
}