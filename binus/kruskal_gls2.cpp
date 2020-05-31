/*
 * Name: Nauval Muhammad Firdaus
 * NIM: 2301906331
 * Class: LD01(Kelas Kecil) / CA01(Kelas Besar)
 
 * Kruskal algorithm, using alphabet as vertex
 * Vertex are 0 based index
 * The graph is considered as undirected graph
*/

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

int find(subset *subs[], int i) 
{
    if (subs[i]->parent != i) subs[i]->parent = find(subs, subs[i]->parent);
    return subs[i]->parent;
}

void set_union(subset *subs[], int x, int y) 
{
    int xroot = find(subs, x);
    int yroot = find(subs, y);

    if (subs[xroot]->rank > subs[yroot]->rank) 
    {
        subs[yroot]->parent = xroot;
    } else if (subs[yroot]->rank > subs[xroot]->rank) 
    {
        subs[xroot]->parent = yroot;
    } else 
    {
        subs[yroot]->parent = xroot;
        subs[xroot]->rank += 1;
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
int kruskal(edge *edges[], int V, int E) 
{
	// Bubble sort
	for (int i = 0; i < E; i++) 
	{
		for (int j = i + 1; j < E; j++) 
		{
			if (edges[i]->w > edges[j]->w) 
			{
				edge *temp = edges[i];
				edges[i] = edges[j];
				edges[j] = temp;
			}
		}
	}
	
	printf("Sorted edges from smalles weight to largest:\n");
	printf("+----------+----------+--------+\n");
	printf("| Vertex 1 | Vertex 2 | weight |\n");
	printf("+----------+----------+--------+\n");
	for (int i = 0; i < E; i++) 
	{
		printf("| %-8c | %-8c | %-6d |\n", idx_to_alpha(edges[i]->u), idx_to_alpha(edges[i]->v), edges[i]->w);
    	printf("+----------+----------+--------+\n");
	}
	printf("\n\n");
	
    subset *subs[V];
    for (int i = 0; i < V; i++) 
    {
    	subs[i] = (subset*) malloc (sizeof(subset));
        subs[i]->parent = i;
        subs[i]->rank = 0;
    }

    int ans = 0, r1, r2, isLoop;
    
	edge *mst_edge[E];
    int mst_edge_c = 0;
    
    for (int i = 0; i < E; i++) 
    {
    	printf("Current edge %c <--> %c (weight: %d).\n", idx_to_alpha(edges[i]->u), idx_to_alpha(edges[i]->v), edges[i]->w);
    	isLoop = 1;
    	
        r1 = find(subs, edges[i]->u);
        r2 = find(subs, edges[i]->v);

        if (r1 != r2) 
        {
        	isLoop = 0;
        	
            set_union(subs, edges[i]->u, edges[i]->v);
            ans += edges[i]->w;

			mst_edge[mst_edge_c++] = edges[i];
        }
        
        printf("Cause loop: %s\n", (isLoop) ? "True" : "False");
        if (!isLoop) printf("Add edge to minimum spanning tree (MST).\n");
        printf("\n\n");
    }
    
    // Free subsets
    for (int i = 0; i < V; i++) 
	{
		free(subs[i]);
		subs[i] = NULL;
	}
	
	printf("Kruskal Algorithm, edges that construct MST\n");
	printf("===========================================\n\n");
	for (int i = 0; i < mst_edge_c; i++) 
	{
		printf("%c <--> %c (weight: %d).\n", idx_to_alpha(mst_edge[i]->u), idx_to_alpha(mst_edge[i]->v), mst_edge[i]->w);
	}
	printf("\n");
	
    return ans;
}

int main() 
{
	printf("Kruskal Algorithm Simulation\n");
	printf("============================\n\n");
	
    int V, E;
    V = 8;
    E = 12;
    
    edge *edges[E];
    for (int i = 0; i < E; i++) 
	{
		edges[i] = (struct edge*) malloc (sizeof(edge));
	}

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

	printf("Inputed edges (bidirectional/undirected):\n");
	printf("+----------+----------+--------+\n");
	printf("| Vertex 1 | Vertex 2 | weight |\n");
	printf("+----------+----------+--------+\n");
    for (int i = 0; i < E; i++)
    {
    	printf("| %-8c | %-8c | %-6d |\n", input_v1[i], input_v2[i], input_w[i]);
    	printf("+----------+----------+--------+\n");
    	
        u = input_v1[i];
        v = input_v2[i];
        w = input_w[i];

        edges[i]->u = alpha_to_idx(u);
        edges[i]->v = alpha_to_idx(v);
        edges[i]->w = w;
    }
	printf("\n\n");
	
    ans = kruskal(edges, V, E);
    printf("Total minimum weight in MST: %d\n", ans);
    
    // Free edges
    for (int i = 0; i < E; i++) 
	{
		free(edges[i]);
		edges[i] = NULL;
	}

    return 0;
}
