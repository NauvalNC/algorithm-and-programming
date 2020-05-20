/*
 * Name: Nauval Muhammad Firdaus
 * NIM: 2301906331
 * Class: LD01 (Kelas Kecil) / CA01 (Kelas Besar)
 * 
 * =======================
 * ATTENTION
 * =======================
 * This Red-Black Tree is using predecessor for deleting node with two children.
 * The result may differ with successor version.
 * 
 * This predecessor method satisfy the simulation on 
 * https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
 * 
 * Thank you :)
*/

#include <stdio.h>
#include <stdlib.h>

enum color { BLACK, RED };

typedef struct node 
{
    int key;
    enum color color;
    struct node *left;
    struct node *right;
    struct node *parent;
} node;

typedef struct rbt 
{
    node *root;
} rbt;

typedef struct qnode 
{
    node *val;
    struct qnode *next;
} qnode;

node *make_node(int);

rbt *make_rbt();

qnode *make_qnode(node**);
void push(qnode**, qnode**, node**, int*);
void pop(qnode**, qnode**, int*);
void pop_all(qnode**, qnode**, int*);

void set_root(rbt**, node**);
int is_left_child(node*);
int has_red_child(node*);

node *get_uncle(node*);
node *get_sibling(node*);

void swap_key(node**, node**);
void swap_color(node**, node**);

void left_rot(rbt**, node**);
void right_rot(rbt**, node**);

node *search(rbt*, int);
node *get_replacement(rbt*, node*);

void insert(node**, int);
void insert(rbt**, int);

void fix_redred(rbt**, node**);
void fix_duobleblack(rbt**, node**);

void del_node(rbt**, node**);
void del(rbt**, int);

void level_order(rbt*);

void free_nodes(node**);
void free_rbt(rbt**);

/* Driver - START */
int main() 
{
    rbt *tree = make_rbt();

    char opt;
    int num;

    do 
    {
        scanf("%c", &opt); getchar();
        if (opt == 'e') break;

        scanf("%d", &num); getchar();
        if (opt == 'i') insert(&tree, num);
        else if (opt == 'd') del(&tree, num);

        printf("=== RBT ===\n");
        level_order(tree);
        printf("\n");
    } while (opt != 'e');

    free_rbt(&tree);
}
/* Driver - END */

node *make_node(int key) 
{
    node *temp = (node*) malloc (sizeof(node));
    temp->key = key;
    temp->color = RED;
    temp->left = temp->right = temp->parent = NULL;
    return temp;
}

rbt *make_rbt() 
{
    rbt *temp = (rbt*) malloc (sizeof(rbt));
    temp->root = NULL;
    return temp;
}

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

void set_root(rbt **tree, node **new_root) 
{
    (*tree)->root = *new_root;
    (*tree)->root->color = BLACK;
}

int is_left_child(node *x) { return x == x->parent->left; }

int has_red_child(node *x) 
{ 
    int c_left = (x->left == NULL) ? BLACK : x->left->color;
    int c_right = (x->right == NULL) ? BLACK : x->right->color;

    return (c_left == RED || c_right == RED) ? 1 : 0;
}

node *get_uncle(node *x) 
{
    if (x->parent == NULL || x->parent->parent == NULL) return NULL;

    if (is_left_child(x->parent)) return x->parent->parent->right;
    else return x->parent->parent->left;
}

node *get_sibling(node* x) 
{
    if (x->parent == NULL) return NULL;
    return (is_left_child(x)) ? x->parent->right : x->parent->left;
}

void swap_key(node **a, node **b) 
{
    int key = (*a)->key;
    (*a)->key = (*b)->key;
    (*b)->key = key;
}

void swap_color(node **a, node **b) 
{
    color c_temp = (*a)->color;
    (*a)->color = (*b)->color;
    (*b)->color = c_temp;
}

void left_rot(rbt **tree, node **x) 
{
    node *y = (*x)->right;
    node *b = y->left;

    y->parent = (*x)->parent;
    if (*x == (*tree)->root) set_root(tree, &y);
    else
    {
        if (is_left_child(*x)) (*x)->parent->left = y;
        else (*x)->parent->right = y;
    }

    (*x)->parent = y;
    y->left = *x;

    (*x)->right = b;
    if (b != NULL) b->parent = *x;

    *x = y;
}

void right_rot(rbt **tree, node **x)
{
    node *y = (*x)->left;
    node *b = y->right;

    y->parent = (*x)->parent;
    if (*x == (*tree)->root) set_root(tree, &y);
    else
    {
        if (is_left_child(*x)) (*x)->parent->left = y;
        else (*x)->parent->right = y;
    }

    (*x)->parent = y;
    y->right = *x;

    (*x)->left = b;
    if (b != NULL) b->parent = *x;

    *x = y;
}

node *search(rbt *tree, int key) 
{
    if (tree == NULL) return NULL;
    
    node *temp = tree->root;
    while (temp != NULL) 
    {
        if (key < temp->key) 
        {
            temp = temp->left;
        } else if (key > temp->key) 
        {
            temp = temp->right;
        } else break;
    }

    return temp;
}

node *get_replacement(rbt *tree, node *x) 
{
    if (x->left == NULL && x->right == NULL) return NULL;
    if (x->left == NULL && x->right != NULL) return x->right;
    if (x->left != NULL && x->right == NULL) return x->left;
    
    node *pred = x->left;
    while(pred != NULL && pred->right != NULL) pred = pred->right;
    return pred;
}

void fix_redred(rbt **tree, node **x) 
{
    if (*x == (*tree)->root) 
    {
        (*x)->color = BLACK;
        return;
    }

    node *parent = (*x)->parent;
    node *gparent = parent->parent;
    node *uncle = get_uncle(*x);

    // Red-red violation
    if (parent->color == RED) 
    {
        // Uncle is red, recoloring case
        if (uncle != NULL && uncle->color == RED) 
        {
            parent->color = BLACK;
            uncle->color = BLACK;
            gparent->color = RED;
            fix_redred(tree, &gparent);
        }

        // Uncle is black, rotation and recoloring case
        else 
        {
            if (is_left_child(parent)) 
            {
                if (is_left_child(*x)) 
                {
                    // Left-left case
                    swap_color(&parent, &gparent);
                } else 
                {
                    // Left-right case
                    left_rot(tree, &parent);
                    swap_color(x, &gparent);
                }
                right_rot(tree, &gparent);
            } else 
            {
                if (is_left_child(*x)) 
                {
                    // Right-left case
                    right_rot(tree, &parent);
                    swap_color(x, &gparent);
                } else 
                {
                    // Right-right case
                    swap_color(&parent, &gparent);
                }
                left_rot(tree, &gparent);
            }
        }
    }
}

void insert(rbt **tree, node **x, int key) 
{
    node *new_node = NULL;

    if (*x == NULL) 
    {
        new_node = make_node(key);
        set_root(tree, &new_node);
    } else if (key < (*x)->key) 
    {
        if ((*x)->left == NULL) 
        {
            new_node = make_node(key);
            new_node->parent = *x;
            (*x)->left = new_node;
        } else 
        {
            insert(tree, &((*x)->left), key);
        }
    } else if (key > (*x)->key) 
    {
        if ((*x)->right == NULL) 
        {
            new_node = make_node(key);
            new_node->parent = *x;
            (*x)->right = new_node;
        } else 
        {
            insert(tree, &((*x)->right), key);
        }
    }

    if (new_node != NULL) fix_redred(tree, &new_node);
}

void insert(rbt **tree, int key) 
{
    if (*tree == NULL) return;
    insert(tree, &((*tree)->root), key);
}

void fix_doubleblack(rbt **tree, node **x) 
{
    if (*x == (*tree)->root) return;

    node *sibling = get_sibling(*x);
    node *parent = (*x)->parent;

    // If no sibling, push double black up
    if (sibling == NULL) 
    {
        fix_doubleblack(tree, &parent);
    } 
    else 
    {
        if (sibling->color == RED) 
        {
            parent->color = RED;
            sibling->color = BLACK;

            if (is_left_child(sibling)) right_rot(tree, &parent);
            else left_rot(tree, &parent);
            
            fix_doubleblack(tree, x);
        } else if (sibling->color == BLACK) 
        {
            // At least one child
            if (has_red_child(sibling)) 
            {
                node *o_parent = parent;

                if (sibling->left != NULL && sibling->left->color == RED) 
                {
                    // Left-left case
                    if (is_left_child(sibling)) 
                    {
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        right_rot(tree, &parent);
                    } 
                    // Left-right case
                    else 
                    {
                        sibling->left->color = parent->color; 
                        right_rot(tree, &sibling); 
                        left_rot(tree, &parent); 
                    }
                } else
                {
                    // Right-left case
                    if (is_left_child(sibling)) 
                    {
                        sibling->right->color = parent->color; 
                        left_rot(tree, &sibling); 
                        right_rot(tree, &parent); 
                    } 
                    // Right-right case
                    else 
                    {
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        left_rot(tree, &parent);
                    }
                }
                o_parent->color = BLACK;
            } 
            
            // Two black children
            else 
            {
                sibling->color = RED;
                if (parent->color == BLACK) fix_doubleblack(tree, &parent);
                else parent->color = BLACK;
            }
        }
    }
}

void del_node(rbt **tree, node **v) 
{
    node *u = get_replacement(*tree, *v);

    int uvBlack = ((u == NULL || u->color == BLACK) && (*v)->color == BLACK);
    node *parent = (*v)->parent;

    // If v is leaf
    if (u == NULL) 
    {
        // If v is root
        if (*v == (*tree)->root) 
        {
            free(*v);
            (*tree)->root = *v = NULL;
        } else 
        {
            if (uvBlack) fix_doubleblack(tree, v);
            
            // Recolor sibling
            node *sibling = get_sibling(*v);
            if (sibling != NULL) sibling->color = RED;

            // Perform deletion
            if (is_left_child(*v)) parent->left = NULL;
            else parent->right = NULL;
            free(*v); *v = NULL;
        }
    }

    // If v has one child
    else if (((*v)->left == NULL && (*v)->right != NULL) ||
        ((*v)->left != NULL && (*v)->right == NULL)) 
    {
        // If v is root
        if (*v == (*tree)->root) 
        {
            (*v)->key = u->key;
            (*v)->left = (*v)->right = NULL;
            free(u);
        } else 
        {
            u->parent = parent;
            if (is_left_child(*v)) parent->left = u;
            else parent->right = u;
            free(*v); *v = NULL;

            if (uvBlack) fix_doubleblack(tree, v);
            else u->color = BLACK;
        }
    }

    // If v has two children
    else 
    {
        swap_key(v, &u);
        del_node(tree, &u);
    }
}

void del(rbt **tree, int key) 
{
    if (*tree == NULL || (*tree)->root == NULL) return;
    
    node *v = search(*tree, key);
    
    if (v == NULL) 
    {
        printf("Node not found.\n");
        return;
    }

    del_node(tree, &v);
}

void level_order(rbt *tree) 
{
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
        while(c_size--) 
        {
            node *c_node = head->val;
            pop(&head, &tail, &size);

            if (c_node == NULL) printf(" nil(b)");
            else printf(" %d(%c)", c_node->key, c_node->color == BLACK ? 'b' : 'r');

            if (c_node != NULL) 
            {
                push(&head, &tail, &(c_node->left), &size);
                push(&head, &tail, &(c_node->right), &size);
            }
        }
        printf("\n");
    }

    pop_all(&head, &tail, &size);
}

void free_nodes(node **x) 
{
    if (*x == NULL) return;
    free_nodes(&((*x)->left));
    free_nodes(&((*x)->right));
    free(*x);
    *x = NULL;
}

void free_rbt(rbt **tree) 
{
    if (*tree == NULL) return;
    free_nodes(&(*tree)->root);
    free(*tree);
    *tree = NULL;
}
