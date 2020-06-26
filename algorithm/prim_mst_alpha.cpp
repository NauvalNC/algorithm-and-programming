/*
 * MST Prim's Algorithm
 * Using 0 Index based, using uppercase alphabet 'A' as 0 index
 * Better use Kruskal
*/

#include <stdio.h>
#include <stdlib.h>

const int INF = 1000000;
const int VMAX = 10005;

int atoidx(char alpha) { return ((int)alpha) - 65; }
char idxtoa(int idx) { return (char)(idx + 65);}

int V, E;

int adj[VMAX][VMAX];

int prim()
{
	int mst = 0;
	
	int num_edge = 0;
	
	// Track selected vertex and its visited status
	int visited[V];
	
	for (int i = 0; i < V; i++) visited[i] = 0;
	
	// Set first vertex as starting vertex
	visited[0] = 1;
	
	int row, col;
	
	printf("MST egdes\n");
	
	while(num_edge < V - 1) 
	{
		int min = INF;
		row = col = 0;
		
		// Loop through vertex
		for (int i = 0; i < V; i++) 
		{
			// If the vertex is currently selected/visisted
			if (visited[i]) 
			{
				// Loop to find the adjacent vertex
				for (int j = 0; j < V; j++) 
				{
					// If vertex j is not visited and adjacent to current vertex
					// Means the edge is not already in MST yet.
					if (visited[j] == 0 && adj[i][j]) 
					{
						// If the weight is smaller then the current minimum weight
						// Update the minimum edge with minimum edge
						if (min > adj[i][j]) 
						{
							min = adj[i][j];
							row = i;
							col = j;
						}
					}
				}
			}
		}
		
		printf("%c - %c : %d\n", idxtoa(row), idxtoa(col), adj[row][col]);
		
		// Set the adjacent vertex to selected/visited
		visited[col] = 1;
		
		num_edge += 1;
		
		mst += adj[row][col];
	}
	
	printf("Total MST: %d\n", mst);
	
	return mst;
}

typedef struct edge 
{
	int u, v, w;	
} edge;

int main() 
{
	V = 8;
	E = 12;
	
	edge e[E] = 
	{
		{'A', 'H', 8},
		{'A', 'D', 12},
		{'A', 'G', 5},
		{'H', 'G', 3},
		{'G', 'C', 21},
		{'D', 'E', 19},
		{'G', 'E', 7},
		{'H', 'B', 12},
		{'B', 'C', 22},
		{'G', 'F', 25},
		{'H', 'E', 17},
		{'E', 'F', 10}
	};
		
	int u, v, w;
	for (int i = 0; i < E; i++) 
	{
		u = atoidx(e[i].u);
		v = atoidx(e[i].v);
		w = e[i].w;
		
		// Assign adjacent
		adj[u][v] = w;
		adj[v][u] = w;	
	}
	
	prim();
	
	return 0;
}
