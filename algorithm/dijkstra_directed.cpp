// Solve for SPOJ EZDIJKST

#include <stdio.h>
#include <stdlib.h>

#define INF 1000000
#define MAX 1000000

typedef struct vertex 
{
    int adj[MAX];
    int cost[MAX];
    int c_adj;
} vertex;

int dijkstra(vertex *verts[], int V, int E, int vA, int vB) 
{
    int w_path[V + 1];
    int visited[V + 1];

    for (int i = 0; i <= V; i++) 
    {
        w_path[i] = INF;
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
            }
        }
        
        int min_vert = -1;
        int min_cost = INF;
        for (int i = 0; i <= V; i++) 
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

    return w_path[vB];
}

int main() 
{
    int cases;
    int V, E;
    int u, v, w;
    int vA, vB;
    int ans;

    scanf("%d", &cases);
    while(cases--) 
    {
        scanf("%d %d", &V, &E);
        
        vertex *verts[V + 1];
        for (int i = 0; i <= V; i++) 
        {
            verts[i] = (vertex*) malloc (sizeof(vertex));
            verts[i]->c_adj = 0;
        }

        for (int i = 0; i < E; i++) 
        {
            scanf("%d %d %d", &u, &v, &w);

            // Create adjacent
            verts[u]->adj[verts[u]->c_adj] = v;
            verts[u]->cost[verts[u]->c_adj] = w;
            verts[u]->c_adj += 1;
        }

        scanf("%d %d", &vA, &vB);

        ans = dijkstra(verts, V, E, vA, vB);
        if (ans == INF) printf("NO\n");
        else printf("%d\n", ans);

        // Free verts
        for (int i = 0; i <= V; i++) 
        {
            free(verts[i]);
            verts[i] = NULL;
        }
    }

    return 0;
}
