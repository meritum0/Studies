#include <iostream>
#include <vector>

#define MOD 1000000000

using namespace std;

int main(int argc, char *argv[]) 
{
	ios_base::sync_with_stdio(false);
	
	cin.tie(NULL);
	
	int n;
	cin >> n;
	
	if ( n == 1 )
	{
		cout << 1;
		return 0;
	}
	
	vector<int> input;
	for ( int i = 0; i < n; i++ )
	{
		int x;
		cin >> x;
		input.push_back(x);
	}
	
	vector<vector<int64_t>> lewa(input.size(), vector<int64_t>(input.size(), 0)), prawa(input.size(), vector<int64_t>(input.size(), 0));
	
	for ( int i = 0; i < input.size(); i++ )
	{
		lewa[i][i] = 1;
		prawa[i][i] = 1;
	}
	
	for ( int i = 2; i <= input.size(); i++ )
	{
		for ( int j = 0; i + j <= input.size(); j++ )
		{
			if ( input[j] < input[j+1] )
				lewa[j][i+j-1] += lewa[j+1][i+j-1];
			if ( input[j] < input[i+j-1] )
				lewa[j][i+j-1] += j+1 < i+j-1 ? prawa[j+1][i+j-1] : 0;
			
			if ( input[i+j-1] > input[i+j-2] )
				prawa[j][i+j-1] += prawa[j][i+j-2];
			if ( input[i+j-1] > input[j] )
				prawa[j][i+j-1] += j < i+j-2 ? lewa[j][i+j-2] : 0;
			
			lewa[j][i+j-1] %= MOD;
			prawa[j][i+j-1] %= MOD;
		}
	}
	
	cout << (lewa[0][input.size()-1] + prawa[0][input.size()-1]) % MOD;
}