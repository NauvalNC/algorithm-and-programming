/*
 * Name: Nauval Muhammad Firdaus
 * Class: LD01 (Kelas Kecil) / CA01 (Kelas Besar)
 * NIM: 2301906331
 * ===============
 * Attention
 * ===============
 * This AVL Tree uses predecessor as replacement for deleting node with two children.
 * The result may differ with successor version.
 * 
 * This AVL Tree satisfy the simulation on
 * https://www.cs.usfca.edu/~galles/visualization/AVLtree.html
 * 
 * Thank you :)
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node 
{
    int key;
    int height;
    struct node *left, *right, *parent;
} node;

typedef struct avl 
{
    node *root;
} avl;

typedef struct qnode 
{
    node *val;
    struct qnode *next;
} qnode;

qnode *make_qnode(node** val) 
{
    qnode *temp = (qnode*) malloc (sizeof(qnode));
    temp->val = *val;
    temp->next = NULL;
    return temp;
}

void push(qnode **head, qnode **tail, node **val, int *size) 
{
    qnode *new_node = make_qnode(val);

    if (*head == NULL) 
    {
        *head = *tail = new_node;
    } else 
    {
        (*tail)->next = new_node;
        *tail = new_node;
    }

    (*size)++;
}

void pop(qnode **head, qnode **tail, int *size) 
{
    if (*head == NULL) return;

    qnode *tdel = *head;
    *head = (*head)->next;
    free(tdel);

    if (*head == NULL) *tail = NULL;

    (*size)--;
}

void pop_all(qnode **head, qnode** tail, int *size) 
{
    while (*head != NULL) pop(head, tail, size);
}

node *make_node(int key) 
{
    node *temp = (node*) malloc (sizeof(node));
    temp->key = key;
    temp->height = 1;
    temp->left = temp->right = temp->parent = NULL;
    return temp;
}

avl *make_avl() 
{
    avl *temp = (avl*) malloc (sizeof(avl));
    temp->root = NULL;
    return temp;
}

int get_height(node *x) 
{
    if (x == NULL) return 0;
    return x->height;
}

int get_bfactor(node *x) 
{
    if (x == NULL) return 0;
    return get_height(x->left) - get_height(x->right);
}

int max(int a, int b) { return (a > b) ? a : b; }

int is_left_child(node *x) 
{
    if (x == NULL || x->parent == NULL) return 1;
    return x->parent->left == x;
}

void left_rot(avl **tree, node **x) 
{
    node *parent = (*x)->parent;
    int x_left = is_left_child(*x);

    node *y = (*x)->right;
    node *b = y->left;

    y->parent = (*x)->parent;

    y->left = *x;
    (*x)->parent = y;

    (*x)->right = b;
    if (b != NULL) b->parent = *x;

    // Update height
    (*x)->height = max(get_height((*x)->left), get_height((*x)->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    *x = y;

    if (parent == NULL) (*tree)->root = *x;
    else 
    {
        if (x_left) parent->left = *x;
        else parent->right = *x;
    }
}

void right_rot(avl **tree, node **x) 
{
    node *parent = (*x)->parent;
    int x_left = is_left_child(*x);

    node *y = (*x)->left;
    node *b = y->right;

    y->parent = (*x)->parent;

    y->right = *x;
    (*x)->parent = y;

    (*x)->left = b;
    if (b != NULL) b->parent = *x;

    // Update height
    (*x)->height = max(get_height((*x)->left), get_height((*x)->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    *x = y;

    if (parent == NULL) (*tree)->root = *x;
    else 
    {
        if (x_left) parent->left = *x;
        else parent->right = *x;
    }
}

void rebalance(avl **tree, node **x) 
{
    // Update node height
    (*x)->height = max(get_height((*x)->left), get_height((*x)->right)) + 1;

    int bfactor = get_bfactor(*x);

    // Left weighted
    if (bfactor > 1) 
    {
        // Left-left case
        if (get_bfactor((*x)->left) >= 0) 
        {
            right_rot(tree, x);
        } 
        // Left-right case
        else 
        {
            left_rot(tree, &(*x)->left);
            right_rot(tree, x);
        }
    } 

    // Right weighted
    else if (bfactor < -1) 
    {
        // Right-right case
        if (get_bfactor((*x)->right) <= 0) 
        {
            left_rot(tree, x);
        } 
        // Right-left case
        else 
        {
            right_rot(tree, &(*x)->right);
            left_rot(tree, x);
        }
    }
}

void insert(avl **tree, node **x, int key) 
{
    node *new_node = NULL;

    if (key < (*x)->key) 
    {
        if ((*x)->left == NULL) 
        {
            new_node = make_node(key);
            (*x)->left = new_node;
            new_node->parent = *x;
        } else 
        {
            insert(tree, &(*x)->left, key);
        }
    } else if (key > (*x)->key) 
    {
        if ((*x)->right == NULL) 
        {
            new_node = make_node(key);
            (*x)->right = new_node;
            new_node->parent = *x;
        } else 
        {
            insert(tree, &(*x)->right, key);
        }
    }

    rebalance(tree, x);
}

void insert(avl **tree, int key) 
{
    if (*tree == NULL) return;
    
    if ((*tree)->root == NULL) (*tree)->root = make_node(key);
    else insert(tree, &(*tree)->root, key);
}

void del(avl **tree, node **x, int key) 
{
    if (*x == NULL) 
    {
        printf("Not found.\n");
        return;
    }

    if (key < (*x)->key) 
    {
        del(tree, &(*x)->left, key);
    } else if (key > (*x)->key) 
    {
        del(tree, &(*x)->right, key);
    } else 
    {
        node *tdel = *x;
        node *parent = (*x)->parent;

        if ((*x)->left == NULL && (*x)->right == NULL) 
        {
            free(tdel);
            *x = NULL;
        } else if ((*x)->left != NULL && (*x)->right == NULL) 
        {
            *x = (*x)->left;
            free(tdel);
        } else if ((*x)->left == NULL && (*x)->right != NULL) 
        {
            *x = (*x)->right;
            free(tdel);
        } else 
        {
            node *pred = (*x)->left;
            while(pred != NULL && pred->right != NULL) pred = pred->right;

            (*x)->key = pred->key;

            del(tree, &(*x)->left, (*x)->key);
        }
    }

    if (*x != NULL) rebalance(tree, x);
}

void del(avl **tree, int key) 
{
    if (*tree == NULL) return;
    del(tree, &(*tree)->root, key);
}

void free_nodes(node **x) 
{
    if (*x == NULL) return;
    free_nodes(&(*x)->left);
    free_nodes(&(*x)->right);
    free(*x);
    *x = NULL;
}

void free_avl(avl **tree) 
{
    if (*tree == NULL) return;
    free_nodes(&(*tree)->root);
    free(*tree);
    *tree = NULL;
}

void level_order(avl *tree) 
{
    printf("== Level order ==\n");
    if (tree == NULL || tree->root == NULL) 
    {
        printf("No nodes in tree.\n");
        return;
    }

    qnode *head = NULL, *tail = NULL;
    int size = 0, level = 1;

    push(&head, &tail, &(tree->root), &size);

    while(head != NULL) 
    {
        int c_size = size;
        printf("Level %d:", level++);
        while (c_size--)
        {
            node *curr = head->val;
            pop(&head, &tail, &size);

            printf(" %d(%dh)", curr->key, curr->height);

            if (curr->left != NULL) push(&head, &tail, &(curr->left), &size);
            if (curr->right != NULL) push(&head, &tail, &(curr->right), &size);
        }
        printf("\n");
    }

    pop_all(&head, &tail, &size);
}

void in_order(node *x)
{
    if (x == NULL) return;
    in_order(x->left);
    printf(" %d", x->key);
    in_order(x->right);
}

int main() 
{
    avl *tree = make_avl();

    char opt;
    int num;

    while(1) 
    {
        scanf("%c", &opt); getchar();
        if (opt == 'e') break;

        scanf("%d", &num); getchar();
        if (opt == 'i') 
        {
            insert(&tree, num);
        } else if (opt == 'd') 
        {
            del(&tree, num);
        }

        level_order(tree);
        in_order(tree->root);
        printf("\n");
    }

    free_avl(&tree);

    return 0;
}
