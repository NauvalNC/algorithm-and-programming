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

	printf("%c to %c\n", idxtoa(vA), idxtoa(vB));
	printf("| Step | Node   | To Node | Selected |\n");
	printf("|      | %c      |", idxtoa(vA));
	for (int i = 1; i < V; i++) 
	{
		printf(" %-6c |", idxtoa(i));
	}
	printf(" %c |\n", idxtoa(vA));
	
	int step = 1;
	
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
        
        // Print data
        printf("| %-4d | %-3d(%c) |", step++, w_path[vA], idxtoa(f_path[vA]));
		for (int i = 1; i < V; i++) 
		{
			if (w_path[i] == INF) 
			{
				printf(" %-6s |", "INF");	
			} else 
			{
				printf(" %-3d(%c) |", w_path[i], idxtoa(f_path[i]));
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

		// Print selected
        if (min_vert == -1) 
		{
			printf(" %c |\n", ' ');
			break;	
		}
        s_idx = min_vert;
    	printf(" %c |\n", idxtoa(s_idx));
	}

	int ans = w_path[vB];
	
	printf("Minimum cost from %c to %c: ", idxtoa(vA), idxtoa(vB));
    if (ans == INF) printf("Impossible.\n");
    else 
	{
		printf("%d\n", ans);
		printf("Path: ");
		
		int idx = f_path[vB];
		printf("%c <-", idxtoa(vB));
		while (idx != vA) 
		{
			printf("%c <-", idxtoa(idx));
			idx = f_path[idx];
		}
		printf("%c", idxtoa(vA));
	}
    
    return ans;
}

typedef struct edge 
{
	char v;
	char u;
	int w;
} edge;

int main() 
{
    int cases;
    int V, E;
    int v1, v2, w;
    int vA, vB;
    int ans;

    V = 6;
    E = 10;
    
    // Init the vertex
    vertex *verts[V];
    for (int i = 0; i < V; i++) 
    {
        verts[i] = (vertex*) malloc (sizeof(vertex));
        verts[i]->c_adj = 0;
    }
	
    // Inputing
    edge e[E] = 
	{
		{'A', 'B', 3},
		{'A', 'C', 6},
		{'B', 'C', 3},
		{'B', 'D', 6},
		{'B', 'E', 7},
		{'C', 'E', 6},
		{'C', 'D', 2},
		{'D', 'E', 2},
		{'E', 'F', 5},
		{'D', 'F', 2},
	};

	// Change to check the input
	int check_input = 0;
	
    for (int i = 0; i < E; i++) 
    {
        v1 = atoidx(e[i].v);
        v2 = atoidx(e[i].u);
        w = e[i].w;
		
    	if (check_input) printf("%c %c %d\n", idxtoa(v1), idxtoa(v2), w);
    	
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

    return 0;
}
