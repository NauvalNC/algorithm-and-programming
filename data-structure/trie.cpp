#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int alpha = 26;

typedef struct node 
{
    char key;
    int is_word;
    int occur;
    node *child[alpha];
} node;

node *make_node(char key) 
{
    node *temp = (node*) malloc (sizeof(node));
    temp->key = key;
    temp->is_word = 0;
    temp->occur = 0;
    for (int i = 0; i < alpha; i++) temp->child[i] = NULL;
    return temp;
}

char to_lower(char x) 
{
    if ((int)x >= (int)'a' && (int)x <= (int)'z') return x;
    return (char)((int)x + ((int)'a' - (int)'A'));
}

int convert_to_pos(char x) 
{
    return (int)to_lower(x) - (int)'a';
}

void insert(node *x, char *str) 
{
    node *curr = x;
    int len = strlen(str);
    int pos;

    for (int i = 0; i < len; i++) 
    {
        pos = convert_to_pos(str[i]);

        if (curr->child[pos] == NULL) 
        {
            curr->child[pos] = make_node(str[i]);
        }
        curr->child[pos]->occur += 1;

        curr = curr->child[pos];
    }
    curr->is_word = 1;
}

int search(node *x, char *str) 
{
    node *curr = x;
    int len = strlen(str);
    int pos;

    for (int i = 0; i < len; i++) 
    {
        pos = convert_to_pos(str[i]);
        if (curr->child[pos] == NULL) return 0;
        curr = curr->child[pos];
    }

    if (curr != NULL) return curr->is_word;
    return 0;
}

int get_occur(node *x, char *str) 
{
    node *curr = x;
    int len = strlen(str);
    int pos;

    for (int i = 0; i < len; i++) 
    {
        pos = convert_to_pos(str[i]);
        if (curr->child[pos] == NULL) return 0;
        curr = curr->child[pos];
    }

    if (curr != NULL) return curr->occur;
    return 0;
}

void free_nodes(node **x) 
{
    if (*x == NULL) return;
    
    for (int i = alpha - 1; i >= 0; i--) 
    {
        free_nodes(&(*x)->child[i]);
    }

    free(*x);
    *x = NULL;
}

int main() 
{
    node *trie = make_node('*');

    int cases;
    char opt[10], str[105];

    scanf("%d", &cases); getchar();
    while(cases--) 
    {
        scanf("%s %s", opt, str); getchar();
        if (opt[0] == 'a') 
        {
            insert(trie, str);
        } else 
        {
            printf("is_word: %d\n", search(trie, str));
            printf("occur: %d\n", get_occur(trie, str));
        }
    }

    free_nodes(&trie);

    return 0;
}
