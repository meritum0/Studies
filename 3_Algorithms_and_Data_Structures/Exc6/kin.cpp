#include <iostream>
#include <vector>

using namespace std;

void add_elem( vector<int64_t> &tree, int pos, int64_t to_add )
{
	while (pos > 0)
	{
		tree[pos] += to_add % 1000000000;
		tree[pos] = tree[pos] % 1000000000;
		pos /= 2;
	}
}

int64_t get_sum( vector<int64_t> &tree, int left, int right )
{
	if ( left > right )
		return 0;
	
	int64_t sum = 0;
	sum += tree[left] % 1000000000;
	sum = sum % 1000000000;
	
	if ( left != right )
	{
		sum += tree[right] % 1000000000;
		sum = sum % 1000000000;
	}
	
	while ( left/2 != right/2 )
	{
		if ( left%2 == 0 )	//lewe dziecko
		{
			sum += tree[left+1] % 1000000000;
			sum = sum % 1000000000;
		}
		if ( right%2 == 1 )
		{
			sum += tree[right-1] % 1000000000;
			sum = sum % 1000000000;
		}
		
		left /= 2;
		right /= 2;
	}
	return sum;
}

int get_tree_size(int n)
{
	int pow = 1;
	while ( pow/2 < n )
		pow *= 2;
	
	return pow;
}

int64_t compute( vector<int> &perm, int k )
{
	int tree_size = get_tree_size(perm.size());
	vector<vector<int64_t>> dp(perm.size(), vector<int64_t>(k+1, 0));
	
	for ( int i = 0; i < perm.size(); i++ )
		dp[i][1] = 1;
	
	for ( int l = 2; l <= k; l++ )
	{
		vector<int64_t> tree(tree_size, 0);
		for ( int i = 0; i < perm.size(); i++ )
		{
			dp[i][l] = get_sum(tree, tree_size/2+perm[i], tree_size-1);
			dp[i][l] = dp[i][l] % 1000000000;
			add_elem(tree, perm[i] + tree_size/2 - 1, dp[i][l-1] );
		}
	}
	
	int64_t res = 0;
	for ( int i = 0; i < perm.size(); i++ )
	{
		res = (res + dp[i][k] % 1000000000) % 1000000000;
	}
	
	return res;
}

int main(int argc, char *argv[]) {
	int n, k;
	cin >> n;
	cin >> k;
	
	vector<int> perm(n);
	
	for ( int i = 0; i < n; i++ )
		cin >> perm[i];
	
	cout << compute(perm, k);
}