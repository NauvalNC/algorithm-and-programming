#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node 
{
    char value[101];
    struct node *next;
    struct node *prev;
    struct node *tail;
};

void pushTail(struct node **localHead, const char *value) 
{
    struct node *newNode = (struct node*) malloc (sizeof(struct node));
    strcpy(newNode->value, value);
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*localHead == NULL) 
    {
        //Set the newNode node as head and tail node
        *localHead = newNode;
        (*localHead)->tail = newNode;
    }
    else 
    {
        //Set the newNode node as tail node 
        newNode->prev = (*localHead)->tail;
        (*localHead)->tail->next = newNode;
        (*localHead)->tail = newNode;
    }
}

void popTail(struct node **localHead) 
{
    if (*localHead == NULL) return;

    struct node *temp = (*localHead)->tail;
    (*localHead)->tail = (*localHead)->tail->prev;
    
    if ((*localHead)->tail != NULL) (*localHead)->tail->next = NULL;

    free(temp);
}

void printData(struct node **localHead) 
{
    struct node *curr = *localHead;
    
    int iter = 1;
    while (curr != NULL)
    {
        printf("%d. %s\n", iter++, curr->value);
        curr = curr->next;
    }
}

void nextTab(struct node **curr) 
{
    if (*curr == NULL) return;

    printf("Current tab: %s\n", (*curr)->value);

    if ((*curr)->next == NULL) return;
    *curr = (*curr)->next;
    
    printf("Current after altered to next: %s\n", (*curr)->value);
}

void prevTab(struct node **curr) 
{
    if (*curr == NULL) return;

    printf("Current tab: %s\n", (*curr)->value);

    if ((*curr)->prev == NULL) return;
    *curr = (*curr)->prev;
    
    printf("Current after altered to next: %s\n", (*curr)->value);
}

void currTab(struct node **curr)
{
    if (*curr == NULL) return;

    printf("Current tab: %s\n", (*curr)->value);
}

void freeAll(struct node **localHead) 
{
    while (*localHead != NULL) popTail(localHead);
}

int main() 
{
    struct node *doubleLinkedList = NULL;

    pushTail(&doubleLinkedList, "https://google.com");
    pushTail(&doubleLinkedList, "https://facebook.com");
    pushTail(&doubleLinkedList, "https://twitter.com");
    pushTail(&doubleLinkedList, "https://pinterest.com");

    popTail(&doubleLinkedList);

    printf("All Tabs:\n");
    printData(&doubleLinkedList);
    puts("");

    //Set current tab as tail tab
    struct node *current = doubleLinkedList->tail;

    currTab(&current);
    puts("");

    prevTab(&current);
    puts("");

    prevTab(&current);
    puts("");

    nextTab(&current);
    puts("");

    currTab(&current);

    return 0;

    return 0;
}