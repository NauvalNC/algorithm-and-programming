#include <stdio.h>
#include <stdlib.h>

#define ll long long int

typedef struct heap 
{
    ll arr[100005];
    int cap;
    int size;
} heap;

heap *make_heap() 
{
    heap *temp = (heap*) malloc (sizeof(heap));
    temp->cap = sizeof(temp->arr) / sizeof(temp->arr[0]);
    temp->size = 0;
    return temp;
}

int get_parent(int idx) { return (idx - 1) / 2; }
int get_left(int idx) { return (2 * idx) + 1; }
int get_right(int idx) { return (2 * idx) + 2; }

void swap(ll *a, ll *b) 
{
    ll temp = *a;
    *a = *b;
    *b = temp;
}

void insert(heap **heap, ll key) 
{
    if ((*heap)->size == 0) 
    {
        (*heap)->arr[0] = key;
        (*heap)->size += 1;
        return;
    }

    int idx = (*heap)->size;
    (*heap)->arr[idx] = key;
    (*heap)->size += 1;

    // Heapify
    while(1) 
    {
        if (idx == 0) break;

        int parent = get_parent(idx);
        int left = get_left(parent);
        int right = get_right(parent);
        int less = parent;

        if (left < (*heap)->size && (*heap)->arr[left] < (*heap)->arr[less]) less = left;
        if (right < (*heap)->size && (*heap)->arr[right] < (*heap)->arr[less]) less = right;

        if (less != parent) 
        {
            swap(&(*heap)->arr[less], &(*heap)->arr[parent]);
            idx = parent;
        } else break;
    }
}

void del(heap **heap, ll key) 
{
    if ((*heap)->size <= 0) return;

    int idx = -1;
    for (int i = 0; i < (*heap)->size; i++) 
    {
        if ((*heap)->arr[i] == key) 
        {
            idx = i;
            break;
        }
    }

    if (idx == -1) return;

    // Swap with last element
    swap(&(*heap)->arr[idx], &(*heap)->arr[(*heap)->size - 1]);
    (*heap)->size -= 1;

    // Heapify
    while (1)
    {
        if (idx >= (*heap)->size) return;

        int left = get_left(idx);
        int right = get_right(idx);
        int less = idx;

        if (left < (*heap)->size && (*heap)->arr[left] < (*heap)->arr[less]) less = left;
        if (right < (*heap)->size && (*heap)->arr[right] < (*heap)->arr[less]) less = right;

        if (less != idx) 
        {
            swap(&(*heap)->arr[less], &(*heap)->arr[idx]);
            idx = less;
        } else break;
    }   
}

void free_heap(heap **heap) 
{
    free(*heap);
    *heap = NULL;
}

int main() 
{
    heap *heap = make_heap();

    int cases;
    ll key;
    int opt;

    scanf("%d", &cases);
    while(cases--) 
    {
        scanf("%d", &opt);
        if (opt == 3) 
        {
            if (heap->size > 0) printf("%lld\n", heap->arr[0]);
            continue;
        }

        scanf("%lld", &key);
        if (opt == 1) 
        {
            insert(&heap, key);
        } else if (opt == 2) 
        {
            del(&heap, key);
        }
    }

    free_heap(&heap);
    
    return 0;
}
