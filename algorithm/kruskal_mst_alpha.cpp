 
#include <stdio.h>
#include <stdlib.h>

int alpha_to_idx(char alpha) 
{
    return (int)alpha - 65;
}

char idx_to_alpha(int idx) 
{
    return (char)(idx + 65);
}

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
            printf("%c <--> %c (weight: %d)\n", 
                idx_to_alpha(edges[i].u),
                idx_to_alpha(edges[i].v), 
                edges[i].w);
        }
    }

    if (ans == 0) printf("Impossible to construct MST.\n");

    return ans;
}

int main() 
{
    int V, E;
    V = 8;
    E = 12;
    
    edge edges[E];

    char u, v;
    int w, ans;

    // Inputing
    char input_v1[] = 
    {
        'A',
        'A',
        'A',
        'H',
        'H',
        'H',
        'G',
        'G',
        'G',
        'D',
        'B',
        'E'
    };
    char input_v2[] = 
    {
        'H',
        'G',
        'D',
        'B',
        'G',
        'F',
        'F',
        'E',
        'C',
        'E',
        'C',
        'F'
    };
    int input_w[] 
    {
        8,
        5,
        12,
        12,
        3,
        17,
        25,
        7,
        21,
        19,
        22,
        10
    };

    for (int i = 0; i < E; i++)
    {
        u = input_v1[i];
        v = input_v2[i];
        w = input_w[i];

        edges[i].u = alpha_to_idx(u);
        edges[i].v = alpha_to_idx(v);
        edges[i].w = w;
    }

    ans = kruskal(edges, V, E);
    printf("Total minimum weight in MST: %d\n", ans);

    return 0;
}
