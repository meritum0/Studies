#include <iostream>
#include <set>
#include <vector>

using namespace std;

vector<int64_t> tab;

struct Cmp {
	bool operator()(pair<int,int> a, pair<int,int> b) const { return a.first > b.first; }	// znak<
};

set<pair<int,int>, Cmp> intervals;

typedef struct elem {
	int best;
	pair<int, int> most_left;
	pair<int, int> most_right;
} elem;

typedef struct custom_res {
	int best;
	pair<int, int> most_left;
	pair<int, int> most_right;
} custom_res;

int get_tree_size(int n)
{
	int pow = 1;
	while ( pow/2 < n )
		pow *= 2;
	
	return pow;
}

void init(vector<elem> &tree, int idx, int left, int right, int n)
{
	if (idx >= tree.size()/2 + n) return;
	
	tree[idx].most_left = (pair<int,int>){left, right > n ? n : right};
	tree[idx].most_right = (pair<int,int>){left, right > n ? n : right};
	tree[idx].best = (right > n ? n : right) - left + 1;
	
	if (idx < tree.size()/2)
	{
		init(tree, idx*2, left, (left+right)/2, n);
		init(tree, idx*2 + 1, (left+right)/2 + 1, right, n);
	}
}

bool zawiera(int a, int b, int left, int right)
{
	return a <= left && b >= right;
}

custom_res znajdz_ciag(vector<elem> &tree, int idx, int left, int right, int a, int b)
{
	if (zawiera(a, b, left, right))
	{
		return (custom_res){tree[idx].best, tree[idx].most_left, tree[idx].most_right};
	}

	custom_res left_res, right_res, res;
	if (a <= (left + right)/2)	// do lewego
	{
		left_res = znajdz_ciag(tree, idx*2, left, (left+right)/2, a, b);
	}
	
	if (b > (left + right)/2)	// do prawego
	{
		right_res = znajdz_ciag(tree, idx*2 + 1, (left+right)/2 + 1, right, a, b);
	}

	if (a <= (left + right)/2 && b > (left + right)/2 && tab[right_res.most_left.first-1] <= 0)	// mozna polaczyc w jeden monotoniczny ciag
	{
		if (left_res.most_left == left_res.most_right)
		{
			res.most_left = make_pair(left_res.most_left.first, right_res.most_left.second);
		}
		else
			res.most_left = left_res.most_left;
		
		if (right_res.most_left == right_res.most_right)
		{
			res.most_right = make_pair(left_res.most_right.first, right_res.most_right.second);
		}
		else
			res.most_right = right_res.most_right;
		
		int nowy_ciag = right_res.most_left.second - left_res.most_right.first + 1;
		res.best = max(nowy_ciag, max(left_res.best, right_res.best));
	}
	else if (a <= (left + right)/2 && b > (left + right)/2)
	{
		res.best = max(left_res.best, right_res.best);
		res.most_left = left_res.most_left;
		res.most_right = right_res.most_right;
	}
	else if (b > (left + right)/2)
	{
		return right_res;
	}
	else // a <= (left + right)/2
	{
		return left_res;
	}
	
	return res;
}

void aktualizuj(vector<elem> &tree, int idx, int left, int right, int a, int pocz, int kon, bool czy_merge)
{
	if (idx >= tree.size()/2) return;
	
	pocz = max(pocz, left);
	kon = min(kon, right);
	
	if (a <= (left + right)/2)
	{
		aktualizuj(tree, idx*2, left, (left+right)/2, a, pocz, kon, czy_merge);
	}
	else if (a > (left + right)/2)
	{
		aktualizuj(tree, idx*2 + 1, (left+right)/2 + 1, right, a, pocz, kon, czy_merge);
	}
	
	if (czy_merge)
		{
			if (a == left) return;
			
			if (tree[idx].most_left.second + 1 == a)
			{
				tree[idx].most_left.second = kon;
			}
			if (a == tree[idx].most_right.first)
			{
				tree[idx].most_right.first = pocz;
			}
			
			tree[idx].best = max(kon-pocz+1, tree[idx].best);
		}
	
	else
		{
			if (a == right) return;
			
			// tutaj a traktujemy jako b
			if (zawiera(tree[idx].most_right.first, tree[idx].most_right.second, a, a))
			{
					tree[idx].most_right.first = a+1;
			}
			if (zawiera(tree[idx].most_left.first, tree[idx].most_left.second, a, a))
			{
				tree[idx].most_left.second = a;
			}
			
			tree[idx].best = max(tree[idx*2].best, tree[idx*2+1].best);
			if (tab[tree[idx*2].most_right.second] <= 0)
			{
				tree[idx].best = max(tree[idx].best, tree[idx*2+1].most_left.second - tree[idx*2].most_right.first + 1);
			}
		}
}

void akcja_nawozenia(vector<elem> &tree, int a, int b, int k)
{
	tab[b] += k;
	tab[a-1] -= k;
	
	int64_t l, r, kon, pocz;
	pair<int,int> nowa1, nowa2;
	auto left = intervals.lower_bound({a,a});
	if (left->first == a && a != 1) // byc moze merge
	{
		if (tab[a-1] <= 0)	// merge
		{
			auto temp = left;
			left++;
			
			nowa1 = {left->first, temp->second};
			kon = temp->second;
			pocz = left->first;
			
			
			intervals.erase(temp);
			intervals.erase(left);
			intervals.insert(nowa1);
			
			aktualizuj(tree, 1, 1, tree.size()/2, a, pocz, kon, true);
		}
	}
	
	auto right = intervals.lower_bound({b,b});
	if (right->second > b) // byc moze rozdzielamy
	{
		if (tab[b] > 0) // rozdzielamy
		{
			nowa1 = {right->first, b};
			nowa2 = {b+1, right->second};
			
			intervals.erase(right);
			intervals.insert(nowa1);
			intervals.insert(nowa2);
			
			aktualizuj(tree, 1, 1, tree.size()/2, b, nowa1.first, nowa2.second, false);
		}
	}
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
		
	int n, m, tree_size;
	cin >> n;
	cin >> m;
	vector<int64_t> t(n+1,0);
	tab = t;
	tree_size = get_tree_size(n);
	intervals.insert({1,n});
	vector<elem> tree(tree_size);
	init(tree, 1, 1, tree_size/2, n);
	
	char znak;
	int a, b, k;
	for ( int i = 0; i < m; i++ )
	{
		cin >> znak;
		
		if ( znak == 'N' )
		{
			cin >> a;
			cin >> b;
			cin >> k;
							
			akcja_nawozenia(tree, a, b, k);
		}
		else
		{
			cin >> a;
			cin >> b;
					
			cout << znajdz_ciag(tree, 1, 1, tree_size/2, a, b).best << "\n";
		}
	}
}
