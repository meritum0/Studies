#include <iostream>
#include <vector>

using namespace std;

int64_t maximum( int64_t a, int64_t b ) { return a > b ? a : b; }

void inicjuj( vector<vector<int64_t>> &dp,  vector<vector<int>> &grid, int k )
{
	vector<int64_t> sums( k, 0 );
	
	for ( int mask = 3; mask < (1 << k); mask++ )
	{
		int tile;
		if ( (mask & 0b11) == 0b11) {
			tile = grid[0][0] + grid[0][1];
			sums[1] = tile > 0 ? tile : 0;
		}
		else
		{
			sums[1] = 0;
		}
		
		for ( int i = 2; i < k; i++ )
		{
			if ( (mask & (0b11 << (i-1))) == (0b11 << (i-1)) )
			{
				tile = grid[0][i-1] + grid[0][i];
				sums[i] = maximum( sums[i-1], sums[i-2] + tile );
			}
			else
			{
				sums[i] = sums[i-1];
			}
		}
		
		dp[0][mask] = sums[k-1];
	}
}

int64_t compute( vector<vector<int>> &grid, int n, int k )
{
	vector<vector<int64_t>> dp ( n, vector<int64_t> (1 << k, 0) );
	
	inicjuj( dp, grid, k );
	
	for ( int i = 1; i < n; i++ )
	{
		for ( int mask = 0; mask < (1 << k); mask++ )
		{
			bool next = false;
			for ( int j = 0; j < k; j++ )
			{
				int counter = 0;
				while ( mask & (1 << j) )
				{
					counter++;
					j++;
				}
				if ( counter % 2 == 1 )
				{
					next = true;
					break;
				}
			}
			
			if ( next )
				continue;
			
			int neg_mask = ~mask & ((1 << k) - 1);
			int64_t sum;
			for ( int submask = neg_mask; submask > 0; submask = (submask-1) & neg_mask )
			{
				sum = dp[i-1][~submask & ((1 << k) - 1)];
				
				for ( int j = 0; j < k; j++ )
				{
					int tile = 0;
					if ( mask & (1 << j) && mask & (1 << (j+1)) )
					{
						tile = grid[i][j] + grid[i][j+1];
						if ( tile > 0 )
						{
							sum += tile;
						}
						j++;
					}
					else if ( submask & (1 << j) )
					{
						tile = grid[i-1][j] + grid[i][j];
						if ( tile > 0 )
						{
							sum += tile;
						}
					}
				}
				
				dp[i][mask|submask] = maximum( dp[i][mask|submask], sum );
			}
			
			// submask = 0
			
			sum = dp[i-1][((1 << k) - 1)];
			
			for ( int j = 0; j < k; j++ )
			{
				int tile = 0;
				if ( mask & (1 << j) && mask & (1 << (j+1)) )
				{
					tile = grid[i][j] + grid[i][j+1];
					if ( tile > 0 )
					{
						sum += tile;
					}
					j++;
				}
			}
			
			dp[i][mask] = maximum( dp[i][mask], sum );
			
		}
		
		for ( int mask = 0; mask < (1 << k); mask++ )
		{
			for ( int j = 0; j < k; j++ )
			{
				int submask = mask & ~(1 << j);
				if ( submask != mask )
				{
					dp[i][mask] = maximum( dp[i][mask], dp[i][submask] );
				}
			}
		}
	}
	
	return dp[n-1][(1 << k) - 1];
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	
	std::cin.tie(NULL);
	
	int n, k;
	cin >> n;
	cin >> k;
	
	vector<vector<int>> grid ( n, vector<int> (k) );
	
	for ( int i = 0; i < k; i++ )
	{
		for ( int j = 0; j < n; j++ )
		{
			cin >> grid[j][i];
		}
	}
	
	cout << compute( grid, n, k );
}