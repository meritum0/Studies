#include <iostream>
#include <string>

using namespace std;

int minimum( int a, int b ) { return a < b ? a : b; }

int main()
{
	string input;
	cin >> input;
	
	int min = input.length();
	
	char prev = input[0];
	int luka = 0;
	for ( int i = 0; i < input.length(); i++ ) 
	{
		char cur = input[i];
		
		if ( (prev == '*' || cur == prev) && cur != '*' )
		{
			prev = cur;
			luka = 0;
			continue;
		}
				
		if ( cur != '*' && prev != '*' && cur != prev )
		{
			min = minimum(min, luka);
			luka = 0;
			prev = cur;
		}
		else
		{
			luka++;
		}
	}
	
	int ans = input.length() - min;
	if ( ans == 0 ) ans++;
	cout << ans;
}