/*
 * MST Kruskal Algorithm
 * Using 1 based Index, SPOJ Test using bubble reach 80 points
 * Better use Kruskal
*/

#include <stdio.h>
#include <stdlib.h>

#define ll long long int

ll E, V;

typedef struct edge 
{
	ll u, v;
	ll w;	
} edge;

edge edges[100005];

typedef struct subset 
{
	ll parent;
	ll rank;
} subset;

ll find(ll x, subset subs[]) 
{
	if (subs[x].parent != x) subs[x].parent = find(subs[x].parent, subs);
	return subs[x].parent;
}

void set_union(ll x, ll y, subset subs[]) 
{
	ll xparent = find(x, subs);
	ll yparent = find(y, subs);
	
	if (subs[xparent].rank > subs[yparent].rank) 
	{
		subs[yparent].parent = xparent;
	} else if (subs[yparent].rank > subs[xparent].rank) 
	{
		subs[xparent].parent = yparent;
	} else 
	{
		subs[yparent].parent = xparent;
		subs[xparent].rank += 1;
	}
}

int w_comp(const void *a, const void *b) 
{
    edge *e1 = (edge*)a;
    edge *e2 = (edge*)b;
    return e1->w > e2->w;
} 
    
ll kruskal() 
{
	qsort(edges, E, sizeof(edges[0]), w_comp);
	
	// Sort
//	for (int i = 0; i < E; i++) 
//	{
//		for (int j = i + 1; j < E; j++) 
//		{
//			if (edges[j].w < edges[i].w) 
//			{
//				edge temp = edges[i];
//				edges[i] = edges[j];
//				edges[j] = temp;
//			}
//		}
//	}

	subset subs[V];
	for (ll i = 0; i < V; i++) 
	{
		subs[i].parent = i;
		subs[i].rank = 0;
	}
	
	ll ans = 0;
	for (ll i = 0; i < E; i++) 
	{
		ll uparent = find(edges[i].u, subs);
		ll vparent = find(edges[i].v, subs);
		
		if (uparent != vparent) 
		{
			set_union(edges[i].u, edges[i].v, subs);
			ans += edges[i].w;
		}
	}
	
	return ans;
}

int main() 
{
	scanf("%lld %lld", &V, &E);
	
	ll u, v, w;
	for (ll i = 0; i < E; i++) 
	{
		scanf("%lld %lld %lld", &u, &v, &w);
		
		// SPOJ using 1 index, make it 0 index
		edges[i].u = u - 1;
		edges[i].v = v - 1;
		
		edges[i].w = w;
	}
	
	printf("%lld\n", kruskal());
	
	return 0;
}
