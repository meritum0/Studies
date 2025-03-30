#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <climits>

using namespace std;

bool cmp(pair<pair<int,int>, int> a, pair<pair<int,int>, int> b)
{
	return a.first.second < b.first.second || (a.first.second == b.first.second && a.first.first < b.first.first);
}

class QCMP
{
public:
	bool operator()(pair<int,int> a, pair<int,int> b)
	{
		return b.first < a.first || (b.first == a.first && b.second < a.second);
	}
};

int abs(int a)
{
	if (a < 0) return -a;
	return a;
}

int oblicz(vector<pair<pair<int,int>, int>> &wyspy, pair<int,int> start, pair<int,int> finish)
{
	auto x = wyspy;
	auto y = wyspy;
	sort(x.begin(), x.end());
	sort(y.begin(), y.end(), cmp);
	
	vector<vector<int>> graph(wyspy.size(), vector<int>(0));
	for (int i = 0; i < x.size()-1; i++)
	{
		graph[x[i].second].push_back(x[i+1].second);
		graph[x[i+1].second].push_back(x[i].second);
		
		graph[y[i].second].push_back(y[i+1].second);
		graph[y[i+1].second].push_back(y[i].second);
	}
	
	vector<int> odl(wyspy.size());
	for (int i = 0; i < wyspy.size(); i++)
	{
		odl[wyspy[i].second] = INT_MAX;
	}
	
	odl[0] = 0;
	priority_queue<pair<int, int>, vector<pair<int,int>>, QCMP> pq;
	pq.push({0, 0});
	
	while (!pq.empty())
	{
		auto el = pq.top();
		pq.pop();
		
		if (el.first > odl[el.second])
			continue;
		
		for (auto t : graph[el.second])
		{
			int dist = el.first + min(abs(wyspy[el.second].first.second-wyspy[t].first.second), abs(wyspy[el.second].first.first-wyspy[t].first.first));
			
			if (dist < odl[t])
			{
				odl[t] = dist;
				pq.push({dist, t});
			}
		}
	}
	
	return odl[wyspy.size()-1];
}

int main(int argc, char *argv[]) {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	
	int n;
	cin >> n;
	
	vector<pair<pair<int,int>, int>> wyspy(n);
	
	pair<int,int> start, finish;
	
	cin >> start.first;
	cin >> start.second;
	wyspy[0] = {start,0};
	for (int i = 1; i < n-1; i++)
	{
		pair<int,int> p;
		cin >> p.first;
		cin >> p.second;
		wyspy[i] = {p,i};
	}
	cin >> finish.first;
	cin >> finish.second;
	
	wyspy[n-1] = {finish,n-1};
	
	cout << oblicz(wyspy, start, finish);
}