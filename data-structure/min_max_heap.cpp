#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ll long long int

const int MAX = 1000000;
const int STRLN = 10;

typedef struct node 
{
    ll key;
    int isMin;
    char str[STRLN];
} node;

typedef struct heap 
{
    node data[MAX];
    ll cap;
    ll size;
} heap;

heap *make_heap() 
{
    heap *temp = (heap*) malloc (sizeof(heap));
    temp->cap = sizeof(temp->data) / sizeof(temp->data[0]);
    temp->size = 0;
    return temp;
}

void free_heap(heap **hp) 
{
    if (*hp == NULL) return;
    free(*hp);
    *hp = NULL;
}

ll get_left(ll idx) { return idx * 2; }
ll get_right(ll idx) { return (idx * 2) + 1; }
ll get_parent(ll idx) { return idx / 2; }
ll get_gparent(ll idx) { return get_parent(get_parent(idx)); }

int is_parent_min(heap *hp, ll idx) 
{
    return hp->data[get_parent(idx)].isMin;
}

void swap(node *n1, node *n2) 
{
    int l1 = (*n1).isMin;
    int l2 = (*n2).isMin;

    node temp = *n1;
    *n1 = *n2;
    *n2 = temp;

    // Return the original level
    (*n1).isMin = l1;
    (*n2).isMin = l2;
}

// Get smallest or largest key from children and grandchildren
ll get_most_c(heap *hp, ll idx, int isSmallest) 
{
    ll size = hp->size;

    ll left = get_left(idx);
    ll right = get_right(idx);

    ll left_left = get_left(left);
    ll left_right = get_right(left);

    ll right_left = get_left(right);
    ll right_right = get_right(right);

    ll tg = idx;

    if (isSmallest) 
    {
        if (left <= size && hp->data[tg].key > hp->data[left].key) tg = left;
        if (right <= size && hp->data[tg].key > hp->data[right].key) tg = right;

        if (left_left <= size && hp->data[tg].key > hp->data[left_left].key) tg = left_left;
        if (left_right <= size && hp->data[tg].key > hp->data[left_right].key) tg = left_right;

        if (right_left <= size && hp->data[tg].key > hp->data[right_left].key) tg = right_left;
        if (right_right <= size && hp->data[tg].key > hp->data[right_right].key) tg = right_right;
    } else 
    {
        if (left <= size && hp->data[tg].key < hp->data[left].key) tg = left;
        if (right <= size && hp->data[tg].key < hp->data[right].key) tg = right;

        if (left_left <= size && hp->data[tg].key < hp->data[left_left].key) tg = left_left;
        if (left_right <= size && hp->data[tg].key < hp->data[left_right].key) tg = left_right;

        if (right_left <= size && hp->data[tg].key < hp->data[right_left].key) tg = right_left;
        if (right_right <= size && hp->data[tg].key < hp->data[right_right].key) tg = right_right;
    }

    return tg;
}

node *get_min(heap *hp) 
{
    if (hp->size > 0) return &hp->data[1]; 
    else return NULL;
}

node *get_max(heap *hp) 
{
    if (hp->size < 2) return get_min(hp);
    else if (hp->size == 2) 
    {
        return &hp->data[2];
    } else if (hp->size >= 3) 
    {
        if (hp->data[2].key > hp->data[3].key) return &hp->data[2];
        else return &hp->data[3];
    } else return NULL;
}

void up_heap(heap **hp, ll idx) 
{
    ll parent = get_parent(idx);
    ll gparent = get_gparent(idx);

    // If reach root, return
    if (parent < 1) return;

    // Min Level
    if ((*hp)->data[idx].isMin) 
    {
        if (parent >= 1 && (*hp)->data[idx].key > (*hp)->data[parent].key) 
        {
            swap(&(*hp)->data[idx], &(*hp)->data[parent]);
            up_heap(hp, parent);
        } else if (gparent >= 1 && (*hp)->data[idx].key < (*hp)->data[gparent].key) 
        {
            swap(&(*hp)->data[idx], &(*hp)->data[gparent]);
            up_heap(hp, gparent);
        }
    }

    // Max Level
    else 
    {
        if (parent >= 1 && (*hp)->data[idx].key < (*hp)->data[parent].key) 
        {
            swap(&(*hp)->data[idx], &(*hp)->data[parent]);
            up_heap(hp, parent);
        } else if (gparent >= 1 && (*hp)->data[idx].key > (*hp)->data[gparent].key) 
        {
            swap(&(*hp)->data[idx], &(*hp)->data[gparent]);
            up_heap(hp, gparent);
        }
    }
}

void down_heap(heap **hp, ll idx, int isMin) 
{
    // If out of bound, return
    if (idx > (*hp)->size) return;

    ll rep, parent;

    // Min Level
    if (isMin) 
    {
        // Get smallest
        rep = get_most_c(*hp, idx, 1);

        if (rep != idx) 
        {
            parent = get_parent(rep);

            // Swap rep with current node
            if (rep <= (*hp)->size && (*hp)->data[rep].key < (*hp)->data[idx].key)
                swap(&(*hp)->data[rep], &(*hp)->data[idx]);

            // Rep is grandchildren
            if (parent != idx) 
            {
                if (rep <= (*hp)->size && (*hp)->data[parent].key < (*hp)->data[rep].key)
                    swap(&(*hp)->data[rep], &(*hp)->data[parent]);

                down_heap(hp, rep, isMin);
            }    
        }
    }

    // Max Level
    else 
    {
        // Get largest
        rep = get_most_c(*hp, idx, 0);

        if (rep != idx) 
        {
            parent = get_parent(rep);
            
            // Swap rep with current node
            if (rep <= (*hp)->size && (*hp)->data[rep].key > (*hp)->data[idx].key)
                swap(&(*hp)->data[rep], &(*hp)->data[idx]);

            // Rep is grandchildren
            if (parent != idx) 
            {
                if (rep <= (*hp)->size && (*hp)->data[parent].key > (*hp)->data[rep].key)
                    swap(&(*hp)->data[rep], &(*hp)->data[parent]);

                down_heap(hp, rep, isMin);
            }
        }
    }
}

void insert(heap **hp, ll key, const char *str) 
{
    // Reach cap
    if ((*hp)->size >= (*hp)->cap) return;

    // Increase the size
    (*hp)->size += 1;

    if ((*hp)->size <= 1) 
    {
        // Init
        (*hp)->data[1].key = key;
        (*hp)->data[1].isMin = 1;

        strcpy((*hp)->data[1].str, str);
    } else 
    {
        // Insert
        ll idx = (*hp)->size;
        (*hp)->data[idx].key = key;
        (*hp)->data[idx].isMin = (is_parent_min(*hp, idx)) ? 0 : 1;

        strcpy((*hp)->data[idx].str, str);

        // Up Heap
        up_heap(hp, idx);
    }
}

void del(heap **hp, ll key) 
{
    // Heap is empty
    if ((*hp)->size <= 0) return;

    // Find the index of key
    ll idx = -1;
    for (ll i = 1; i <= (*hp)->size; i++) 
    {
        if ((*hp)->data[i].key == key) 
        {
            idx = i;
            break;
        }
    }

    // Not Found
    if (idx == -1) return;

    // Decrease the size
    (*hp)->size -= 1;

    // Swap with last element
    swap(&(*hp)->data[idx], &(*hp)->data[(*hp)->size] + 1);

    // Downheap
    down_heap(hp, idx, (*hp)->data[idx].isMin);
}

void extract_min(heap **hp) 
{
    node *temp = get_min(*hp);
    
    if (temp != NULL) printf("Min: %lld %s\n", temp->key, temp->str);
    else 
    {
        printf("Min: NULL\n");
        return;
    }

    del(hp, temp->key);
}

void extract_max(heap **hp) 
{
    node *temp = get_max(*hp);
    
    if (temp != NULL) printf("Max: %lld %s\n", temp->key, temp->str);
    else 
    {
        printf("Max: NULL\n");
        return;
    }

    del(hp, temp->key);
}

int main() 
{
    heap *hp = make_heap();

    insert(&hp, 27, "Doso");
    insert(&hp, 12, "Publa");
    insert(&hp, 28, "Saitama");
    insert(&hp, 8, "Naruto");
    insert(&hp, 9, "Genos");
    insert(&hp, 26, "Bang");
    insert(&hp, 22, "Shoma");
    insert(&hp, 30, "Hurul");
    insert(&hp, 35, "Omanko");
    insert(&hp, 14, "Ochinchin");
    insert(&hp, 50, "Nani");
    insert(&hp, 5, "Hello");

    extract_min(&hp);
    extract_max(&hp);
    extract_min(&hp);
    extract_min(&hp);
    extract_max(&hp);
    extract_min(&hp);
    extract_max(&hp);
    extract_max(&hp);
    
    printf("Size: %lld\n", hp->size);

    free_heap(&hp);

    return 0;
}
