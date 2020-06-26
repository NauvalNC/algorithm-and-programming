/*
 * MST Prim's Algorithm
 * Using 1 Index, SPOJ Test Time Limit on running at 10 cases
 * Better use Kruskal
*/

#include <stdio.h>
#include <stdlib.h>

const int INF = 1000000;
const int VMAX = 10005;

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
		
		// Set the adjacent vertex to selected/visited
		visited[col] = 1;
		
		num_edge += 1;
		
		mst += adj[row][col];
	}
	
	return mst;
}

int main() 
{
	scanf("%d %d", &V, &E);
	
	int u, v, w;
	
	for (int i = 0; i < E; i++) 
	{
		scanf("%d %d %d", &u, &v, &w);
		
		// SPOJ using 1 index, make it 0 index
		u -= 1;
		v -= 1;
		
		// Assign adjacent
		adj[u][v] = w;
		adj[v][u] = w;	
	}
	
	printf("%d\n", prim());
	
	return 0;
}
