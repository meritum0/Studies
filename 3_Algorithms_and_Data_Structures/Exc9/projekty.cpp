#include <iostream>
#include <vector>
#include <queue>

using namespace std;

uint64_t maxi(uint64_t a, uint64_t b) { return a >= b ? a : b; }

struct Node
{
	uint64_t programisci;
	int dow; // dowiazania;
};

class Compare
{
public:
	bool operator() (const pair<Node, int> &a, const pair<Node, int> &b)
	{
		return a.first.programisci > b.first.programisci || (a.first.programisci == b.first.programisci && a.second >= b.second);
	}
};

uint64_t compute( vector<Node> &wierz, vector<vector<int>> &zal, int k )
{
	int n = wierz.size();
	uint wybrane = 0;
	priority_queue<pair<Node, int>, vector<pair<Node, int>>, Compare> pq;
	
	for ( int i = 0; i < n; i++ )
	{
		if ( wierz[i].dow == 0 )
		{
			pq.push(make_pair(wierz[i], i));
		}
	}
	
	uint64_t res = 0;
	while ( wybrane < k )
	{
		auto el = pq.top();
		pq.pop();
		wybrane++;
		res = maxi(res, el.first.programisci);
		for ( auto kr : zal[el.second] )
		{
			wierz[kr].dow--;
			if ( wierz[kr].dow == 0 )
				pq.push( make_pair(wierz[kr], kr) );
		}
	}
	
	return res;
}

int main(int argc, char *argv[]) {
	int n, m, k;
	cin >> n;
	cin >> m;
	cin >> k;
	
	vector<Node> wierz(n);
	vector<vector<int>> zal(n);
	
	for ( int i = 0; i < n; i++ )
	{
		uint64_t a;
		cin >> a;
		Node el = (Node){a, 0};
		wierz[i] = el;
	}
	
	for ( int i = 0; i < m; i++ )
	{
		int a, b;
		cin >> a;
		cin >> b;
		zal[b-1].push_back(a-1);
		wierz[a-1].dow++;
	}
	
	cout << compute(wierz, zal, k);
}