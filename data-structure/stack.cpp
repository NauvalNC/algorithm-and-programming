#include <stdio.h>
#include <stdlib.h>

struct node 
{
    int value;
    struct node *next;
};

//Push as top node
void push(struct node **localHead, int value) 
{
    struct node *newNode = (struct node*) malloc (sizeof(struct node));
    newNode->value = value;
    newNode->next = NULL;

    //Set the newNode node as head node
    if (*localHead == NULL) *localHead = newNode;
    else 
    {
        newNode->next = *localHead;
        (*localHead) = newNode;
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
    struct node *myStack = NULL;
    push(&myStack, 10);
    push(&myStack, 20);
    push(&myStack, 30);
    push(&myStack, 40);
    push(&myStack, 50);

    pop(&myStack);

    printDatas(&myStack);

    freeAll(&myStack);

    return 0;
}