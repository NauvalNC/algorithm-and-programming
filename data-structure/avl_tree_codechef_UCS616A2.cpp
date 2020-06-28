/*
 * AVL Tree By Nauval Muhammad Firdaus
 * Tested on OJ AVL CodeChef - UCS616A2
 * WA at 90 score
*/

#include <stdio.h>
#include <stdlib.h>

#define ll long long int

// Change to 1 to use predecessor when delete, or 0 to use successor when delete
// By default using Successor
int usingPred = 0;

typedef struct node 
{
	struct node *left, *right;
	ll height;
	ll key;
} node;

node *make_node(ll key) 
{
	node *temp = (node*) malloc (sizeof(node));
	temp->left = NULL;
	temp->right = NULL;
	temp->height = 0;
	temp->key = key;
	return temp;
}

ll max(ll a, ll b) 
{
	return (a > b) ? a : b;
}

ll get_height(node *x) 
{
	if (x == NULL) return 0;
	return x->height;
}

void set_height(node **x) 
{
	if (*x == NULL) return;
	(*x)->height = max(get_height((*x)->left), get_height((*x)->right)) + 1;
}

ll get_bfactor(node *x) 
{
	if (x == NULL) return 0;
	return get_height(x->left) - get_height(x->right);	
}

void right_rot(node **x)
{
	node *y = (*x)->left;
	node *b = y->right;
	
	(*x)->left = b;
	y->right = *x;
	
	set_height(x);
	set_height(&y);
	
	*x = y;	
}

void left_rot(node **x) 
{
	node *y = (*x)->right;
	node *b = y->left;
	
	(*x)->right = b;
	y->left = *x;
	
	set_height(x);
	set_height(&y);
	
	*x = y;	
}

void rebalance(node **x) 
{
	if (*x == NULL) return;
	
	set_height(x);
	
	ll bfactor = get_bfactor(*x);
	
	if (bfactor > 1) 
	{
		if (get_bfactor((*x)->left) >= 0) 
		{
			//left left
			printf("%lld\n", (*x)->key);
			right_rot(x);
		} else 
		{
			//left right
			printf("%lld\n", (*x)->key);
			left_rot(&(*x)->left);
			right_rot(x);
		}
	} else if (bfactor < -1) 
	{
		if (get_bfactor((*x)->right) <= 0) 
		{
			//right right
			printf("%lld\n", (*x)->key);
			left_rot(x);
		} else 
		{
			//right left
			printf("%lld\n", (*x)->key);
			right_rot(&(*x)->right);
			left_rot(x);
		}
	}
}

void insert(node **x, ll key) 
{
	if (*x == NULL) *x = make_node(key);
	else if (key < (*x)->key) 
	{
		insert(&(*x)->left, key);
	} else if (key > (*x)->key) 
	{
		insert(&(*x)->right, key);
	} else return;
	
	rebalance(x);
}

node *find(node **x, ll key) 
{
	if (*x == NULL) return NULL;
	else if (key < (*x)->key) 
	{
		return find(&(*x)->left, key);
	} else if (key > (*x)->key) 
	{
		return find(&(*x)->right, key);
	} else 
	{
		return *x;
	}
}

void del(node **x, ll key) 
{
	if (*x == NULL) return;
	else if (key < (*x)->key) 
	{
		del(&(*x)->left, key);
	} else if (key > (*x)->key) 
	{
		del(&(*x)->right, key);
	} else 
	{
		if ((*x)->left == NULL && (*x)->right == NULL) 
		{
			free(*x);
			*x = NULL;
		} else if ((*x)->left != NULL && (*x)->right == NULL) 
		{
			node *todel = *x;
			*x = (*x)->left;
			free(todel);
		} else if ((*x)->left == NULL && (*x)->right != NULL) 
		{
			node *todel = *x;
			*x = (*x)->right;
			free(todel);
		} else 
		{
			// Delete using predecessor
			if (usingPred) 
			{
				node *pred = (*x)->left;
				while(pred != NULL && pred->right != NULL) 
				{
					pred = pred->right;
				}
				
				// Copy data
				(*x)->key = pred->key;
				
				del(&(*x)->left, (*x)->key);
			} 
			// Delete using successor
			else 
			{
				node *succ = (*x)->right;
				while(succ != NULL && succ->left != NULL) 
				{
					succ = succ->left;
				}
				
				// Copy data
				(*x)->key = succ->key;
				
				del(&(*x)->right, (*x)->key);
			}
		}
	}
	
	rebalance(x);
}

void pre_order(node *x) 
{
	if (x == NULL) return;
	printf(" %lld", x->key);
	pre_order(x->left);
	pre_order(x->right);
}

void in_order(node *x) 
{
	if (x == NULL) return;
	in_order(x->left);
	printf(" %lld", x->key);
	in_order(x->right);
}

void post_order(node *x) 
{
	if (x == NULL) return;
	post_order(x->left);
	post_order(x->right);
	printf(" %lld", x->key);
}

void free_nodes(node **x) 
{
	if (*x == NULL) return;
	free_nodes(&(*x)->left);
	free_nodes(&(*x)->right);
	free(*x);
	*x = NULL;
}

int main() 
{
	node *avl = NULL;
	
	ll n;
	char opt;
	ll num;
	
	scanf("%lld", &n); getchar();
	
	for (ll i = 0; i < n; i++) 
	{
		scanf("%c %lld", &opt, &num); getchar();
		if (opt == 'i') 
		{
			insert(&avl, num);
		} else 
		{
			del(&avl, num);
		}
	}
	
	pre_order(avl); printf("\n");
	in_order(avl); printf("\n");
	post_order(avl); printf("\n");
	
	free_nodes(&avl);
	
	return 0;
}
