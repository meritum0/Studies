#include <iostream>
#include <algorithm>

using namespace std;

bool again = false;
uint64_t again_idx;
uint64_t again_val;
uint64_t again_k;

// Template class representing a node in the AVL tree
template <typename T> class AVLNode {
public:
	T key; // The value of the node
	AVLNode* left; // Pointer to the left child
	AVLNode* right; // Pointer to the right child
	int height; // Height of the node in the tree
	uint64_t count; // Ilosc wezlow w poddrzewie
	uint64_t ile; // Ilosc wezlow o tej wartosci (sa reprezentowane jako 1 node)
	uint64_t val; // element
	
	// Constructor to initialize a node with a given key
	AVLNode(T k, uint64_t x, uint64_t ki)
	: key(k)
	, left(nullptr)
	, right(nullptr)
	, height(1)
	, count(ki)
	, ile(ki)
	, val(x)
	{
	}
};

// Template class representing the AVL tree
template <typename T> class AVLTree {
private:
	// Pointer to the root of the tree
	AVLNode<T>* root;
	
	// function to get the height of a node
	int height(AVLNode<T>* node)
	{
		if (node == nullptr)
			return 0;
		return node->height;
	}
	
	// function to get the balance factor of a node
	int balanceFactor(AVLNode<T>* node)
	{
		if (node == nullptr)
			return 0;
		return height(node->left) - height(node->right);
	}
	
	// function to perform a right rotation on a subtree
	AVLNode<T>* rightRotate(AVLNode<T>* y)
	{
		if (!y->left) return y;
		AVLNode<T>* x = y->left;
		AVLNode<T>* T2 = x->right;
		
		// Perform rotation
		x->right = y;
		y->left = T2;
		
		// Update heights
		y->height
		= max(height(y->left), height(y->right)) + 1;
		x->height
		= max(height(x->left), height(x->right)) + 1;
		
		// Update dyn
		y->count = y->ile + (y->left ? y->left->count : 0) + (y->right ? y->right->count : 0);
		x->count = x->ile + (x->left ? x->left->count : 0) + (x->right ? x->right->count : 0);
		
		// Return new root
		return x;
	}
	
	// function to perform a left rotation on a subtree
	AVLNode<T>* leftRotate(AVLNode<T>* x)
	{
		if (!x->right) return x;
		AVLNode<T>* y = x->right;
		AVLNode<T>* T2 = y->left;
		
		y->left = x;
		x->right = T2;
		
		// Update heights
		x->height
		= max(height(x->left), height(x->right)) + 1;
		y->height
		= max(height(y->left), height(y->right)) + 1;
		
		// Update dyn
		x->count = x->ile + (x->left ? x->left->count : 0) + (x->right ? x->right->count : 0);
		y->count = y->ile + (y->left ? y->left->count : 0) + (y->right ? y->right->count : 0);
		
		// Return new root
		return y;
	}
	
	// function to insert a new key into the subtree rooted
	// with node
	AVLNode<T>* insert(AVLNode<T>* node, T key, uint64_t val, uint64_t ki)
	{
		// Perform the normal BST insertion
		if (node == nullptr)
			return new AVLNode<T>(key, val, ki);
		
		uint64_t cl = (node->left ? node->left->count : 0);
		uint64_t cr = (node->right ? node->right->count : 0);
		
		if (key < cl)
		{
			node->left = insert(node->left, key, val, ki);
		}
		else if (key == cl)
		{
			if (val == node->val)
				node->ile += ki;
			else
				node->left = insert(node->left, key, val, ki);
		}
		else if (key > cl && key < node->count - cr)
		{
			if (val == node->val)
				node->ile += ki;
			else
			{
				uint64_t p_ile = node->ile - (key - cl);
				uint64_t l_ile = node->ile - p_ile;
				again = true;
				again_idx = l_ile;
				again_val = node->val;
				again_k = l_ile;
				node->ile = ki;
				node->right = insert(node->right, 0, node->val, p_ile);
				node->val = val;
			}
		}
		else if (key == node->count - cr && val == node->val )
			node->ile += ki;
		else if (key <= node->count)
			node->right = insert(node->right, key - node->count + cr, val, ki);
		else
			cout << -1 << "\n";
		
		// Update height of this ancestor node
		node->height = 1
		+ max(height(node->left),
			height(node->right));
		
		// Get the balance factor of this ancestor node
		int balance = balanceFactor(node);
		
		// If this node becomes unbalanced, then there are 4
		// cases
		uint64_t cll, crl;
		
		if (node->left)
		{
			cll = (node->left->left ? node->left->left->count : 0);
		}
		else
		{
			cll = 0;
		}
		
		if (node->right)
		{
			crl = (node->right->left ? node->right->left->count : 0);
		}
		else
		{
			crl = 0;
		}
		
		AVLNode<T>* res = node;
		
		// Left Left Case
		if (balance > 1 && key <= cll)
			res = rightRotate(node);
		
		// Right Right Case
		else if (balance < -1 && key - node->count + cr > crl)
			res = leftRotate(node);
		
		// Left Right Case
		else if (balance > 1 && key > cll) {
			node->left = leftRotate(node->left);
			res = rightRotate(node);
		}
		
		// Right Left Case
		else if (balance < -1 && key - node->count + cr <= crl) {
			node->right = rightRotate(node->right);
			res = leftRotate(node);
		}
		
		// Update dyn
		node->count = node->ile + (node->left ? node->left->count : 0) + (node->right ? node->right->count : 0);
		
		return res;
	}
	
	uint64_t get(AVLNode<T>* node, T j) 	// na pewno node != nullptr
	{
		if (!node) return -1;
		uint64_t cl = (node->left ? node->left->count : 0);
		uint64_t cr = (node->right ? node->right->count : 0);
		
		if (j < cl)
		{
			return get(node->left, j);
		}
		else if (j == 0 || j < node->count - cr)
		{
			return node->val;
		}
		else
		{
			return get(node->right, j - node->count + cr);
		}
	}

	
public:
	// Constructor to initialize the AVL tree
	AVLTree()
	: root(nullptr)
	{
	}
	
	// Function to insert a key into the AVL tree
	void insert(T key, uint64_t val, uint64_t ki)
	{
		root = insert(root, key, val, ki);
		if (again)
		{
			root = insert(root, key - again_idx, again_val, again_k);
			again = false;
		}
	}
	
	uint64_t get(T j) { return get(root, j); }
};

int main(int argc, char *argv[]) 
{
	std::ios_base::sync_with_stdio(false);
	
	std::cin.tie(NULL);
	
	int m;
	cin >> m;
	
	AVLTree<uint64_t> tree;
	
	uint64_t j_, x, k, j, param = 0, n = 0;
	for ( int i = 0; i < m; i++ )
	{
		char znak;
		cin >> znak;
		
		if ( znak == 'i' )
		{
			cin >> j_;
			cin >> x;
			cin >> k;
			
			j = (j_ + param) % (n+1);
			
			tree.insert(j, x, k);
			
			n += k;
		}
		else
		{
			cin >> j_;
			
			j = (j_ + param) % n;
			
			param = tree.get(j);
			cout << param << "\n";
		}
	}
}