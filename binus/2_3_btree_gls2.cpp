/*
 * Name: Nauval Muhammad Firdaus
 * NIM: 2301906331
 * Class: LD01(Kelas Kecil) / CA01(Kelas Besar)
 
 * B-Tree (2-3 Tree), prioritizing predecessor for insertion and deletion violation handling.
 * ==========
 * ATTENTION
 * ==========
 * For some reason, Dev C++ sometimes unable to allocate memory.
 * But this program run without problem on Visual Studio Code using gcc and g++ compiler.
 * Its also run without problem on online compiler such https://www.onlinegdb.com/online_c++_compiler
 * Thank You.
*/

#include <stdio.h>
#include <stdlib.h>

int ceil(double x) 
{
    int a = (int)x;
    if ((double)x > (double)a) return a + 1;
    return a;
}

typedef struct node 
{
    int *keys;
    int n_key;
    int is_leaf;
    node **child;
    node *parent;
} node;

typedef struct btree 
{
    node *root;
    int order;
    int mindeg;
} btree;


/* Queue - START */
typedef struct qnode 
{
    node *val;
    struct qnode *next;
} qnode;

qnode *make_qnode(node**);
void push(qnode**, qnode *, node**, int*);
void pop(qnode**, qnode**, int*);
void pop_all(qnode**, qnode**, int*);
/* Queue - END */


btree *make_btree(int);
node *make_node(int);

void split(btree**, node**);

int is_keys_exceeded(node*, int);

int get_min_n_keys(btree *tree) 
{ 
    int min = tree->mindeg - 1;
    return (min <= 0) ? 1 : min;
}

int find_child_pos_to_key(node*, int);

void insert_key(node**, int, int);
void insert(btree**, node**, int);
void insert(btree**, int);

node *get_pred(btree *tree, node* x, int key) 
{
    int idx = find_child_pos_to_key(x, key);
    if (idx < 0) return NULL;

    node *curr = x->child[idx];
    while(curr != NULL && curr->child[curr->n_key] != NULL) 
    {
        curr = curr->child[curr->n_key];
    }
    return curr;
}

node *get_succ(btree *tree, node* x, int key) 
{
    int idx = find_child_pos_to_key(x, key) + 1;
    if (idx > tree->order) return NULL;

    node *curr = x->child[idx];
    while(curr != NULL && curr->child[0] != NULL) 
    {
        curr = curr->child[0];
    }
    return curr;
}

node *get_left_sibling(btree *tree, node *x) 
{
    if (x->parent == NULL) return NULL;
    
    int idx = find_child_pos_to_key(x->parent, x->keys[0]);
    if (idx - 1 < 0) return NULL;
    return x->parent->child[idx - 1];
}

node *get_right_sibling(btree *tree, node *x) 
{
    if (x->parent == NULL) return NULL;

    int idx = find_child_pos_to_key(x->parent, x->keys[0]);
    if (idx + 1 > tree->order) return NULL;
    return x->parent->child[idx + 1];
}

node *merge(node**, node**);

void del_key_at_index(node**, int);
void del(btree**, node**, int);
void del(btree**, int);

void traverse(node*);
void traverse(btree*);
void print_keys(node*);

void level_order(node*);
void level_order(btree*);

void free_node(node**);
void free_nodes(node**);
void free_btree(btree**);

/* Driver - START*/
int main() 
{
    btree *tree = make_btree(3);

    int input[] = {14, 20, 3, 16, 70, 56, 89, 15, 45, 80, 60};
    int i_size = sizeof(input) / sizeof(input[0]);
    for (int i = 0; i < i_size; i++) 
    {
        insert(&tree, input[i]);
        printf("Insert %d:\n", input[i]);
        level_order(tree);
        printf("\n");

        printf("Traverse:");
        traverse(tree); printf("\n");
    }

    int rm[] = {56, 70, 15, 89, 20, 14};
    int rm_size = sizeof(rm) / sizeof(rm[0]);
    for (int i = 0; i < rm_size; i++) 
    {
        del(&tree, rm[i]);
        printf("Delete %d:\n", rm[i]);
        level_order(tree);
        printf("\n");

        printf("Traverse:");
        traverse(tree); printf("\n");
    }

    free_btree(&tree);

    return 0;
}
/* Driver - END*/


btree *make_btree(int order) 
{
    if (order <= 2) 
    {
        printf("Can't construct btree with order <= 2.\n");
        printf("The generalization of btree is 2-3 tree.\n");
        printf("Please construct linked list for order 1 or BST for order 2.\n");
        return NULL;
    }
    
    btree *temp = (btree*) malloc (sizeof(btree));
    temp->order = order;
    temp->mindeg = ceil((double)order / 2);
    return temp;
}

node *make_node(int order) 
{
    node *temp = (node*) malloc (sizeof(node));
    temp->keys = (int*) malloc (sizeof(int) * order);
    temp->n_key = 0;

    temp->child = (node**) malloc (sizeof(node*) * (order + 1));
    for (int i = 0; i <= order; i++) temp->child[i] = NULL;

    temp->parent = NULL;
    temp->is_leaf = 1;

    return temp;   
}


/* Queue - START*/
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
/* Queue - END*/


int is_keys_exceeded(node *x, int order) 
{
    if (x->n_key > order - 1) return 1;
    return 0;
}

void split(btree **tree, node **x) 
{
    int mid = (*x)->n_key / 2;
    int midKey = (*x)->keys[mid];
    
    node *new_child = make_node((*tree)->order);

    // Copy right keys greater than mid, to new child
    // Copy children greater than mid, to new child
    int len = (*x)->n_key;
    for (int i = mid + 1; i <= len; i++) 
    {
        if (i < len) 
        {
            new_child->keys[i - (mid + 1)] = (*x)->keys[i];
            new_child->n_key += 1;
            
            (*x)->keys[i] = 0;
        }
        
        new_child->child[i - (mid + 1)] = (*x)->child[i];
        if ((*x)->child[i] != NULL) (*x)->child[i]->parent = new_child;
        
        (*x)->n_key -= 1;
        (*x)->child[i] = NULL;
    }

    // Set is not leaf child if the splitted node is not leaf
    if (!(*x)->is_leaf) 
    {
        (*x)->is_leaf = 0;
        new_child->is_leaf = 0;
    }

    // If parent is null, then it is the root, make a new root
    if ((*x)->parent == NULL) 
    {
        node *new_parent = make_node((*tree)->order);
        new_parent->keys[0] = midKey;
        new_parent->n_key += 1;

        new_parent->child[0] = *x;
        (*x)->parent = new_parent;

        new_parent->child[1] = new_child;
        new_child->parent = new_parent;

        new_parent->is_leaf = 0;
        (*tree)->root = new_parent;
    } 
    
    // Else, push mid key to parent
    else 
    {
        node *parent = (*x)->parent;

        insert_key(&(*x)->parent, midKey, 1);

        // Find the inserted mid index
        int idx;
        for (idx = 0; idx < parent->n_key; idx++) if (midKey == parent->keys[idx]) break;
        
        // Link the node
        parent->child[idx] = *x;
        (*x)->parent = parent;

        parent->child[idx + 1] = new_child;
        new_child->parent = parent;

        if (is_keys_exceeded(parent, (*tree)->order)) split(tree, &parent);
    }
}

int find_child_pos_to_key(node *x, int key) 
{
    // Find appropiate index to insert new key
    int idx, len = x->n_key;
    for (idx = 0; idx < len; idx++) 
    {
        if (x->keys[idx] >= key) break;
    }

    return idx;
}

void insert_key(node **x, int key, int move_children)  
{
    if ((*x)->n_key == 0) 
    {
        (*x)->keys[0] = key;
    } else 
    {
        int idx = (*x)->n_key - 1;

        // Find the correct position
        for (idx; idx >= 0; idx--) 
        {
            if ((*x)->keys[idx] < key) break;

            // Move keys
            (*x)->keys[idx + 1] = (*x)->keys[idx];
            
            if (move_children) 
            {
                // Move children
                (*x)->child[idx + 2] = (*x)->child[idx + 1];
                (*x)->child[idx + 1] = (*x)->child[idx];
            }
        }

        // Insert the key
        (*x)->keys[idx + 1] = key;
    }
    
    (*x)->n_key += 1;
}

void insert(btree **tree, node **x, int key) 
{
    if ((*x)->is_leaf) 
    {
        insert_key(x, key, 1);
        if (is_keys_exceeded(*x, (*tree)->order)) split(tree, x);
    } else 
    {
        int idx = find_child_pos_to_key(*x, key);
        insert(tree, &(*x)->child[idx], key);
    }
}

void insert(btree **tree, int key) 
{
    if (*tree == NULL) return;

    if ((*tree)->root == NULL) 
    {
        (*tree)->root = make_node((*tree)->order);
        insert_key(&(*tree)->root, key, 1);
        return;
    }

    insert(tree, &(*tree)->root, key);
}

node *merge(node **a, node **b) 
{
    if (*a == NULL) return *b;
    else if (*b == NULL) return *a;

    int start = (*a)->n_key;
    int res = (*a)->n_key + (*b)->n_key;

    node *child = NULL;

    // Merge the keys and child
    for (int i = start; i <= res; i++) 
    {
        if (i != res) 
        {
            (*a)->keys[i] = (*b)->keys[i - start];
            (*a)->n_key += 1;
        }

        child = (*b)->child[i - start];
        (*a)->child[i + 1] = child;
        if (child != NULL) child->parent = *a;
    }

    // Free the the second node
    free_node(b);

    return *a;
}

void del_key_at_index(node **x, int index) 
{
    for (int i = index + 1; i < (*x)->n_key; i++)
    {
        (*x)->keys[i - 1] = (*x)->keys[i];
    }
    (*x)->n_key -= 1;
}

void del(btree **tree, node **x, int key) 
{
    if (*x == NULL) return;

    int idx = find_child_pos_to_key(*x, key);

    if ((*x)->keys[idx] == key) 
    {
        // If x is leaf
        if ((*x)->is_leaf) 
        {
            if (*x == (*tree)->root || (*x)->n_key > get_min_n_keys(*tree)) 
            {
                del_key_at_index(x, idx);
                if ((*x)->n_key <= 0) free_node(x);
            } else 
            {
                int p_idx, s_idx, flag = 0;
                node *nd = *x;

                while (1) 
                {
                    node *s_left = get_left_sibling(*tree, nd);
                    node *s_right = get_right_sibling(*tree, nd);

                    int to_merge = 0;
                    if (((s_left == NULL) || s_left->n_key <= get_min_n_keys(*tree)) &&
                        ((s_right == NULL) || s_right->n_key <= get_min_n_keys(*tree))) 
                    {
                        to_merge = 1;
                    }
                    
                    del_key_at_index(&nd, idx);
                
                    if (!to_merge) 
                    {
                        if (s_left != NULL && s_left->n_key > get_min_n_keys(*tree)) 
                        {
                            // Delete and insert the parent key down
                            p_idx = find_child_pos_to_key(nd->parent, key) - 1;
                            insert_key(&nd, nd->parent->keys[p_idx], 0);
                            del_key_at_index(&nd->parent, p_idx);

                            // Delete and insert left sibling pred key up
                            s_idx = s_left->n_key - 1;
                            insert_key(&nd->parent, s_left->keys[s_idx], 0);
                            del_key_at_index(&s_left, s_idx);
                            
                            // Shift child to right
                            int len = nd->n_key;
                            for (int i = len; i >= 0; i--) 
                            {
                                if (i == 0) 
                                {
                                    nd->child[0] = NULL;
                                    continue;
                                }
                                nd->child[i] = nd->child[i - 1];
                            }

                            int s_len = s_left->n_key + 1;
                            // Link the node to new parent
                            nd->child[0] = s_left->child[s_len];
                            if (nd->child[0] != NULL) nd->child[0]->parent = nd;
                            s_left->child[s_len] = NULL;
                        }
                        else if (s_right != NULL && s_right->n_key > get_min_n_keys(*tree)) 
                        {
                            // Delete and insert the parent key down
                            p_idx = find_child_pos_to_key(nd->parent, key);
                            insert_key(&nd, nd->parent->keys[p_idx], 0);
                            del_key_at_index(&nd->parent, p_idx);

                            // Delete and insert right sibling succ key up
                            s_idx = 0;
                            insert_key(&nd->parent, s_right->keys[s_idx], 0);
                            del_key_at_index(&s_right, s_idx);

                            // Shift child to left
                            int len = nd->n_key;
                            // Link the node to new parent
                            nd->child[len] = s_right->child[0];
                            if (nd->child[len] != NULL) nd->child[len]->parent = nd;

                            int s_len = s_right->n_key + 1;
                            for (int i = 0; i <= s_len; i++) 
                            {
                                if (i == s_len) 
                                {
                                    s_right->child[s_len] = NULL;
                                    continue;
                                }
                                s_right->child[i] = s_right->child[i + 1];
                            }
                        }

                        return;
                    } 
                    
                    // Merge
                    else 
                    {
                        node *merged = NULL;
                        node *parent = nd->parent;

                        p_idx = find_child_pos_to_key(parent, key) + ((s_left != NULL) ? -1 : 0);
                        
                        int p_key = parent->keys[p_idx];
                        del_key_at_index(&parent, p_idx);

                        if (s_left != NULL) merged = merge(&s_left, &nd);
                        else merged = merge(&nd, &s_right);
                        
                        // Merge parent child
                        for (int i = p_idx + 1; i <= parent->n_key + 1; i++) 
                        {
                            if (i == parent->n_key + 1) parent->child[i] = NULL;
                            else parent->child[i] = parent->child[i + 1];
                        }
                        
                        insert_key(&merged, p_key, 0);

                        // If parent is empty
                        if (parent == NULL || parent->n_key <= 0) 
                        {
                            // Set the new root
                            if (parent == (*tree)->root) 
                            {
                                free_node(&parent);
                                (*tree)->root = merged;
                                return;
                            } 
                            
                            // If not root, iterate to find any violation
                            else 
                            {
                                insert_key(&parent, p_key, 0);
                                nd = parent;
                            }
                        } else return;
                    }
                }
            }
        } 
        // If x is not leaf
        else
        {
            node *pred = get_pred(*tree, *x, key);
            node *succ = get_succ(*tree, *x, key);
            int idx = find_child_pos_to_key(*x, key);
            int r_key;

            if (pred != NULL) 
            {
                r_key = pred->keys[pred->n_key - 1];
                (*x)->keys[idx] = r_key;
                del(tree, &pred, r_key);
            } else if (succ != NULL) 
            {
                r_key = succ->keys[0];
                (*x)->keys[idx] = r_key;
                del(tree, &succ, r_key);
            }
        }
    } else 
    {
        del(tree, &(*x)->child[idx], key);
    }
}

void del(btree **tree, int key) 
{
    if (*tree == NULL) return;
    del(tree, &(*tree)->root, key);
}

void traverse(node *x) 
{
    if (x == NULL) return;

    int len = x->n_key;
    int idx = 0;
    for (idx = 0; idx < len; idx++) 
    {
        traverse(x->child[idx]);
        printf(" %d", x->keys[idx]);
    }

    traverse(x->child[idx]);
}

void traverse(btree *tree) 
{ 
    if (tree == NULL || tree->root == NULL) 
    {
        printf("No nodes in tree.");
    } else traverse(tree->root);
    printf("\n");
}

void print_keys(node *x) 
{
    if (x == NULL || x->n_key <= 0) 
    {
        printf("No keys in node.\n");
        return;
    }

    int len = x->n_key;
    for (int i = 0; i < len; i++) printf(" %d", x->keys[i]);
}

void level_order(node *x) 
{
    if (x == NULL) 
    {
        printf("No nodes in tree.\n");
        return;
    }

    qnode *head = NULL, *tail = NULL;
    int size = 0, level = 1;

    push(&head, &tail, &x, &size);

    while (head != NULL)
    {
        int c_size = size;

        printf("Level %d:\n", level++);
        while(c_size--) 
        {
            node *temp = head->val;
            pop(&head, &tail, &size);

            print_keys(temp);
            if (c_size >= 1) printf(" |");

            int len = temp->n_key;
            for (int i = 0; i <= len; i++) 
            {
                if (temp->child[i] != NULL) push(&head, &tail, &temp->child[i], &size);
            }
        }
        printf("\n");
    }

    pop_all(&head, &tail, &size);
}

void level_order(btree *tree) 
{
    if (tree == NULL || tree->root == NULL) printf("No nodes in tree.\n");
    else level_order(tree->root);
}

void free_node(node** x) 
{
    if (*x == NULL) return;
    
    free((*x)->keys);
    (*x)->keys = NULL;

    free((*x)->child);
    (*x)->child = NULL;

    free(*x);
    *x = NULL;
}

void free_nodes(node **x) 
{
    if (*x == NULL) return;

    int idx = (*x)->n_key;
    for (idx; idx >= 0; idx--) free_nodes(&(*x)->child[idx]);

    free_node(x);
}

void free_btree(btree **tree) 
{
    if (*tree == NULL) return;
    free_nodes(&(*tree)->root);
    free(*tree);
    *tree = NULL;
}
