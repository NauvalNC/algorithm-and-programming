#include <stdio.h>
#include <stdlib.h>

typedef struct subset 
{
    int parent, rank;
} subset;

int find(subset subs[], int i) 
{
    if (subs[i].parent != i) subs[i].parent = find(subs, subs[i].parent);
    return subs[i].parent;
}

void set_union(subset subs[], int x, int y) 
{
    int xroot = find(subs, x);
    int yroot = find(subs, y);

    if (subs[xroot].rank > subs[yroot].rank) 
    {
        subs[yroot].parent = xroot;
    } else if (subs[yroot].rank > subs[xroot].rank) 
    {
        subs[xroot].parent = yroot;
    } else 
    {
        subs[yroot].parent = xroot;
        subs[xroot].rank += 1;
    }
}

typedef struct edge 
{
    int u, v, w;
} edge;

// Compare edge vertices weight
int w_comp(const void *a, const void *b) 
{
    edge *e1 = (edge*)a;
    edge *e2 = (edge*)b;
    return e1->w > e2->w;
}

// MST Kruskal Algorithm
int kruskal(edge edges[], int V, int E) 
{
    qsort(edges, E, sizeof(edges[0]), w_comp);

    subset subs[V];
    for (int i = 0; i < V; i++) 
    {
        subs[i].parent = i;
        subs[i].rank = 0;
    }

    printf("MST edges\n");
    printf("=========\n");

    int ans = 0, r1, r2;
    for (int i = 0; i < E; i++) 
    {
        r1 = find(subs, edges[i].u);
        r2 = find(subs, edges[i].v);

        if (r1 != r2) 
        {
            set_union(subs, edges[i].u, edges[i].v);
            ans += edges[i].w;

            // Print MST edges
            printf("%d -- %d (weight: %d)\n", edges[i].u, edges[i].v, edges[i].w);
        }
    }

    if (ans == 0) printf("Impossible to construct MST.\n");

    return ans;
}

int main() 
{
    int V, E;
    scanf("%d %d", &V, &E);
    
    edge edges[E];

    int u, v, w, ans;
    for (int i = 0; i < E; i++) 
    {
        scanf("%d %d %d", &u, &v, &w);
        edges[i].u = u;
        edges[i].v = v;
        edges[i].w = w;
    }
    printf("\n");

    ans = kruskal(edges, V, E);
    printf("MST: %d\n", ans);

    return 0;
}
