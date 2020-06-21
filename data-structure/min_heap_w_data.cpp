#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ll long long int

const int STRLN = 30;
const int MAX = 1000000;

typedef struct node 
{
    ll key;
    char str[STRLN];
} node;

typedef struct heap 
{
    node data[MAX];
    ll size;
    ll cap;
} heap;

heap *make_heap() 
{
    heap *temp = (heap*) malloc (sizeof(heap));
    temp->size = 0;
    temp->cap = sizeof(temp->data) / sizeof(temp->data[0]);
    return temp;
}

void free_heap(heap **hp) 
{
    if (*hp == NULL) return;
    free(*hp);
    *hp = NULL;
}

ll get_left(ll idx) { return 2 * idx; }
ll get_right(ll idx) { return (2 * idx) + 1; }
ll get_parent(ll idx) { return idx / 2; }

void swap(node *n1, node *n2) 
{
    node temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

void up_heap(heap **hp, ll idx) 
{
    if (idx <= 1) return;

    ll parent = get_parent(idx);
    ll left = get_left(parent);
    ll right = get_right(parent);
    
    ll tg = parent;

    if (left >= 1 && left <= (*hp)->size && (*hp)->data[left].key < (*hp)->data[tg].key) tg = left;
    if (right >= 1 && right <= (*hp)->size && (*hp)->data[right].key < (*hp)->data[tg].key) tg = right;

    if (tg != parent) 
    {
        swap(&(*hp)->data[parent], &(*hp)->data[tg]);
        up_heap(hp, parent);
    }
}

void down_heap(heap **hp, ll idx) 
{
    if (idx >= (*hp)->size) return;

    ll left = get_left(idx);
    ll right = get_right(idx);
    
    ll tg = idx;

    if (left >= 1 && left <= (*hp)->size && (*hp)->data[left].key < (*hp)->data[tg].key) tg = left;
    if (right >= 1 && right <= (*hp)->size && (*hp)->data[right].key < (*hp)->data[tg].key) tg = right;

    if (tg != idx) 
    {
        swap(&(*hp)->data[idx], &(*hp)->data[tg]);
        down_heap(hp, tg);
    }
}

void insert(heap **hp, ll key, const char *str) 
{
    // Cap full
    if ((*hp)->size >= (*hp)->cap) return;

    // Increase size
    (*hp)->size += 1;

    // Insert data
    ll idx = (*hp)->size;
    (*hp)->data[idx].key = key;

    strcpy((*hp)->data[idx].str, str);

    up_heap(hp, idx);
}

void del(heap **hp, ll key) 
{
    // No data in heap
    if ((*hp)->size <= 0) return;

    ll idx = -1;

    for (ll i = 1; i <= (*hp)->size; i++) 
    {
        if ((*hp)->data[i].key == key) 
        {
            idx = i;
            break;
        }
    }

    // Key not found
    if (idx == -1) return;

    // Swap with last element
    swap(&(*hp)->data[idx], &(*hp)->data[(*hp)->size]);

    // Decrease size
    (*hp)->size -= 1;

    down_heap(hp, idx);
}

int main() 
{
    heap *hp = make_heap();

    ll q;
    scanf("%lld", &q);

    int opt;
    ll num;
    while(q--) 
    {
        scanf("%d", &opt);
        if (opt == 3) 
        {
            if (hp->size > 0) printf("%lld\n", hp->data[1].key);
        } else 
        {
            scanf("%lld", &num);
            if (opt == 1) 
            {
                insert(&hp, num, "");
            } else 
            {
                del(&hp, num);
            }
        }
    }

    free_heap(&hp);

    return 0;
}
