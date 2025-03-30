#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	
	cin.tie(NULL);
	
	int n, m;
	cin >> n;
	cin >> m;
	int zawodnik;
	
	vector<long long> v (n, 0);
	
	for ( int i = 0; i < m; i++ )
	{
		for ( int j = 0; j < n; j++ ) 
		{
			cin >> zawodnik;
			v[zawodnik - 1] <<= 1;
			if ( j >= n/2 )
				v[zawodnik - 1] += 1;
		}
	}
	
	sort( v.begin(), v.end() );
	
	int i = 1;
	int prev_idx = 0;
	int res = 1;
	for (; i < n; i++ )
	{
		if ( v[i] != v[prev_idx] )
		{
			res = max( res, i - prev_idx );
			prev_idx = i;
		}
	}
	res = max( res, i - prev_idx );
	
	cout << res;
}