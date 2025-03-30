#include <iostream>
#include <regex>
#include <string>

using namespace std;

#define ZDOBYCIE ( R"(^[A-Z](([A-Za-z]+)|([A-Za-z]*( +[A-Za-z]+)+)) [0-3]$)" )	// Wyrażenie do regex.
#define ODEBRANIE ( R"(^-[A-Z](([A-Za-z]+)|([A-Za-z]*( +[A-Za-z]+)+)) [1-3]$)" )// Wyrażenie do regex.
#define WYPISANIE ( R"(^=[1-9][0-9]{0,5} [1-9][0-9]{0,5} [1-9][0-9]{0,5}$)" )	// Wyrażenie do regex.

namespace {

	#define LICZBA_MEDALI 3	    // są tylko 3 rodzaje - złoty, srebrny i brązowy.
	
	using mapa = unordered_map< string, array<int, LICZBA_MEDALI> >;
	
	void 	odebranie( const string &input, mapa &medale, const int &linia );
	void 	klasyfikacja( const string &input, const mapa &medale );
	void 	przyznaj( const string &input, mapa &medale );
	int64_t obliczPunkty( const string &input, const array<int, LICZBA_MEDALI> );
	bool 	sortuj_po_drugim( const pair<string, int64_t> &p1, const pair<string, int64_t> &p2 );
	string 	wyciagnijKraj( const string &input );
	int  	wyciagnijMedal( const string &input );
	
	// Funkcja do sortowania państw najpierw po ilości punktów, a potem leksykograficznie po nazwie.
	bool sortuj_po_drugim( const pair<string, int64_t> &p1, const pair<string, int64_t> &p2 )
	{
		return p1.second > p2.second || (p1.second == p2.second && p1.first < p2.first);
	}
	
	// Wyciąga kraj z danych na wejściu
	string wyciagnijKraj( const string &input )
	{
		int pozycja_start = input[0] == '-' ? 1 : 0;	// Jeśli jest, to pomijamy znak '-'.
		int pozycja_ostatniej_spacji = input.find_last_of(' ');
		
		return input.substr( pozycja_start, pozycja_ostatniej_spacji - pozycja_start );
	}
	
	// Wyciąga medal z danych na wejściu.
	int wyciagnijMedal( const string &input )
	{
		int pos = input.find_last_of(' ');
		string medal = input.substr( pos + 1 );
		
		return stoi(medal);
	}
	
	void odebranie( const string &input, mapa &medale, const int &linia )
	{
		string kraj = wyciagnijKraj(input);
		
		if ( medale.find(kraj) == medale.end() )
			{
				cout << "ERROR " << linia << "\n";	// Takiego kraju nie ma w bazie danych.
				return;
			}
		
		int ktory_odbieramy = wyciagnijMedal(input);
		
		if ( medale[kraj][ktory_odbieramy - 1] <= 0 )
			{
				cout << "ERROR " << linia << "\n"; // Ten kraj nie posiada medalu, który chcemy odebrać.
				return;
			}
		
		medale[kraj][ktory_odbieramy - 1]--;
	}
	
	// Oblicza punkty na podstawie podanego na wejściu wartościowania.
	int64_t obliczPunkty( const string &input, const array<int, LICZBA_MEDALI> medale )
	{
		int szukaj_od = 1;
		int poz_ostatniej_spacji = input.find_first_of( ' ', szukaj_od );
		int waga_zlota = stoi( input.substr(szukaj_od, poz_ostatniej_spacji - szukaj_od) );
		szukaj_od = poz_ostatniej_spacji + 1;
		
		poz_ostatniej_spacji = input.find_first_of( ' ', szukaj_od );
		int waga_srebra = stoi( input.substr(szukaj_od, poz_ostatniej_spacji - szukaj_od) );
		szukaj_od = poz_ostatniej_spacji + 1;
		
		int waga_brazu = stoi( input.substr(szukaj_od, poz_ostatniej_spacji - szukaj_od) );
		
		return waga_zlota*medale[0] + waga_srebra*medale[1] + waga_brazu*medale[2];
	}
	
	// WYpisuje aktualną klasyfikację przy danym wartościowaniu medali.
	void klasyfikacja( const string &input, const mapa &medale )
	{
		vector<pair<string, int64_t>> punktacja( medale.size() );
		
		int i = 0;
		for ( auto element : medale )
			{
				int64_t punkty = obliczPunkty( input, element.second );
				pair<string, int64_t> para(element.first, punkty);
				punktacja[i++] = para;
			}
		
		sort( punktacja.begin(), punktacja.end(), sortuj_po_drugim );
		
		//wypisz
		int miejsce = 0;
		int przesuniecie_miejsca = 0;	// Jeśli np. trzy kraje mają 2. miejsce, to kolejny ma dopiero 5.
		int64_t punkty_poprzedniego = -1;
		for ( auto element : punktacja )
			{
				if ( punkty_poprzedniego != element.second )
					{
						punkty_poprzedniego = element.second;
						miejsce += przesuniecie_miejsca + 1;
						przesuniecie_miejsca = 0;
					}
				else
					{
						przesuniecie_miejsca++;
					}
				
				cout << miejsce << ". " << element.first << "\n";
			}
	}
	
	void przyznaj( const string &input, mapa &medale )
	{
		int medal = wyciagnijMedal(input);
		string kraj = wyciagnijKraj(input);
		
		if ( medale.find(kraj) == medale.end() )
			{
				array<int, 3> pusta = {0,0,0};
				medale[kraj] = pusta;
			}
		
		if ( medal != 0 )
			medale[kraj][medal - 1]++;
	}
}


int main() 
{
	regex zdobycie(ZDOBYCIE);
	regex zabranie(ODEBRANIE);
	regex wypisanie(WYPISANIE);
	
	mapa medale;
	int linia = 0;
	
	while (true)
		{
			string input;
			getline( cin, input );
			linia++;
			
			if ( cin.eof() )
				break;
			else if ( input[0] == '-' && regex_match(input, zabranie) )
				odebranie( input, medale, linia );
			else if ( input[0] == '=' && regex_match(input, wypisanie) )
				klasyfikacja( input, medale );
			else if ( regex_match(input, zdobycie) )
				przyznaj( input, medale );
			else
				cout << "ERROR " << linia << "\n";		// Nieprawidłowe dane na wejściu.
		}
	return 0;
}
