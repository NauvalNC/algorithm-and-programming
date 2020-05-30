/*
 * Dijkstra algorithm, using alphabet as vertex
 * Vertex are 0 based index
 * The graph is considered as undirected graph
*/

#include <stdio.h>
#include <stdlib.h>

#define INF 1000000
#define MAX 1000000

int atoidx(char alpha) 
{
    return ((int)alpha) - 65;
}

char idxtoa(int idx) 
{
    return (char)(idx + 65);
}

typedef struct vertex 
{
    int adj[MAX];
    int cost[MAX];
    int c_adj;
} vertex;

int dijkstra(vertex *verts[], int V, int E, int vA, int vB) 
{
    int w_path[V];
    int visited[V];
    int f_path[V];

    for (int i = 0; i < V; i++) 
    {
        w_path[i] = INF;
        f_path[i] = i;
        visited[i] = 0;
    }
    w_path[vA] = 0;

    int s_idx = vA;
    while(1) 
    {
        visited[s_idx] = 1;

        for (int i = 0; i < verts[s_idx]->c_adj; i++) 
        {
            int adj = verts[s_idx]->adj[i];
            int w_adj = verts[s_idx]->cost[i];

            if (w_adj + w_path[s_idx] < w_path[adj]) 
            {
                w_path[adj] = w_adj + w_path[s_idx];
                f_path[adj] = s_idx;
            }
        }
        
        int min_vert = -1;
        int min_cost = INF;
        for (int i = 0; i < V; i++) 
        {
            if (!visited[i] && w_path[i] < min_cost) 
            {
                min_vert = i;
                min_cost = w_path[i];
            }
        }

        if (min_vert == -1) break;
        s_idx = min_vert;
    }

    printf("Dijkstra Algorithm (from %c to all vertex)\n", idxtoa(vA));
    printf("==========================================\n");
    printf("+--------+------+------+-------------------+\n");
    printf("| vertex | cost | from | total cost from %c |\n", idxtoa(vA));
    printf("+--------+------+------+-------------------+\n");
    for (int i = 0; i < V; i++) 
    {
        printf("| %-6c | %-4d | %-4c | %-17d |\n", idxtoa(i), w_path[i] - w_path[f_path[i]], idxtoa(f_path[i]), w_path[i]);
    }
    printf("+--------+------+------+-------------------+\n");

    return w_path[vB];
}

int main() 
{
    int cases;
    int V, E;
    int v1, v2, w;
    int vA, vB;
    int ans;

    V = 8;
    E = 12;
    
    // Init the vertex
    vertex *verts[V];
    for (int i = 0; i < V; i++) 
    {
        verts[i] = (vertex*) malloc (sizeof(vertex));
        verts[i]->c_adj = 0;
    }

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
        v1 = atoidx(input_v1[i]);
        v2 = atoidx(input_v2[i]);
        w = input_w[i];

        verts[v1]->adj[verts[v1]->c_adj] = v2;
        verts[v1]->cost[verts[v1]->c_adj] = w;
        verts[v1]->c_adj += 1;

        verts[v2]->adj[verts[v2]->c_adj] = v1;
        verts[v2]->cost[verts[v2]->c_adj] = w;
        verts[v2]->c_adj += 1;
    }

    vA = atoidx('A');
    vB = atoidx('F');

    ans = dijkstra(verts, V, E, vA, vB);
    printf("Minimum cost from %c to %c: ", idxtoa(vA), idxtoa(vB));
    if (ans == INF) printf("Impossible.\n");
    printf("%d\n", ans);

    return 0;
}
