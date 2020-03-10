#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node 
{
    int value;
    struct node *next;
};

void pushHead(struct node **localHead, int value) 
{
    struct node *newNode = (struct node*) malloc (sizeof(struct node));
    newNode->value = value;
    newNode->next = NULL;

    if (*localHead == NULL) *localHead = newNode;
    else 
    {
        newNode->next = *localHead;
        (*localHead) = newNode;
    }
}

void popHead(struct node **localHead) 
{
    if (*localHead == NULL) return;

    struct node *temp = *localHead;
    *localHead = (*localHead)->next;
    free(temp);
}

void printData(struct node **localHead) 
{
    struct node *curr = *localHead;
    
    int iter = 1;
    while (curr != NULL)
    {
        printf("%d. %d\n", iter++, curr->value);
        curr = curr->next;
    }
}

void freeAll(struct node **localHead) 
{
    while (*localHead != NULL) popHead(localHead);
}

int main() 
{
    struct node *singleLinkedList = NULL;
    pushHead(&singleLinkedList, 50);
    pushHead(&singleLinkedList, 40);
    pushHead(&singleLinkedList, 30);
    pushHead(&singleLinkedList, 20);
    pushHead(&singleLinkedList, 10);

    popHead(&singleLinkedList);

    printData(&singleLinkedList);

    freeAll(&singleLinkedList);

    return 0;
}