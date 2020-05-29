#include <stdio.h>

typedef struct subset 
{
    int parent;
    int rank;
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

int main() 
{
    int n, q;
    scanf("%d %d", &n, &q);

    subset subs[n];
    for (int i = 0; i < n; i++) 
    {
        subs[i].parent = i;
        subs[i].rank = 0;
    }

    int opt, x, y, xroot, yroot;
    while(q--) 
    {
        scanf("%d %d %d", &opt, &x, &y);
        switch (opt)
        {
            case 0:
                set_union(subs, x, y);
                break;
            case 1:
                xroot = find(subs, x);
                yroot = find(subs, y);
                printf("%d\n", (xroot != yroot) ? 0 : 1);
                break;
        }
    }
}
