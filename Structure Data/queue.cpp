#include <stdio.h>
#include <stdlib.h>

struct node 
{
    int value;
    struct node *next;
    struct node *tail;
};

//Push as last node
void push(struct node **localHead, int value) 
{
    struct node *newNode = (struct node*) malloc (sizeof(struct node));
    newNode->value = value;
    newNode->next = NULL;

    if (*localHead == NULL) 
    {
        //Set the newNode node as head and tail node
        *localHead = newNode;
        (*localHead)->tail = newNode;
    }
    else 
    {
        //Set the newNode node as tail node 
        (*localHead)->tail->next = newNode;
        (*localHead)->tail = newNode;
    }
}

//Pop the top node
void pop(struct node **localHead) 
{
    if (*localHead == NULL) return;

    struct node *temp = *localHead;
    *localHead = (*localHead)->next;
    free(temp);
}

void printDatas(struct node **localHead) 
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
    while (*localHead != NULL) pop(localHead);
}

int main() 
{
    struct node *myQueue = NULL;
    push(&myQueue, 10);
    push(&myQueue, 20);
    push(&myQueue, 30);
    push(&myQueue, 40);
    push(&myQueue, 50);

    pop(&myQueue);
    
    printDatas(&myQueue);

    freeAll(&myQueue);

    return 0;
}