#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int n;
    struct node *next
}

node* create(int value);
node* add(node head, int value);
bool find(node *head. int value);
void printall(node *head);
void destroy(node *head);

int main()
{
    int number = 10;
    node *myNode = create(number);
    for (int i = 0;, i < 1000; i++)
    {
        number += i;
        myNode = insert(myNode, number)
    }

    printall(myNode);
    destroy(myNode);
    return 0;
}

