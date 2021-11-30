z#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int n;
    struct node *next;
}
node;

void manual_testing();

node* create(int value);                // create a new linked list
node* insert(node *head, int value);    // insert a node into a ll
bool  find(node *head, int value);      // check if a value exists in a LL
void  printall(node *head);             // print all vals in a LL
void  destroy(node *head);              // delete an entire LL
//void  destroynode(node* head);          // delete a single node

int main()
{
    // manual_testing();
    int number = 10;
    node *myNode = create(number);
    for (int i = 0; i < 8; i++)
    {
        number += i;
        myNode = insert(myNode, number);
    }

    printall(myNode);
    destroy(myNode);

    return 0;
}

node* create(int value)
// create a new linked list
// Steps:
// 1. dynamically allocate space for new node
// 2. check to avoid segfault; don't run out of memory
// 3. initialize Val field
// 4. initialize Next field
// 5. return a Pointer to the newly created node
{
    node *newNode = malloc(sizeof(node));   // 1

    if (newNode == NULL)                    // 2
    {
        printf("MALLOC FAIL\n");
    }

    newNode->n = value;                     // 3
    newNode->next = NULL;                   // 4

    return newNode;                         // 5
}

node* insert(node *head, int value)
// insert a node into a ll
// Steps:
// 1. dynamically allocate space for new node
// 2. check to avoid segfault; don't run out of memory
// 3. initialize Val field
// 4. initialize Next field to point to Head node
// 5. return a pointer to this newly created node
{
    node *newNode  = malloc(sizeof(node));  // 1

    if (newNode == NULL)                    // 2
    {
        printf("MALLOC FAIL\n");
    }

    newNode->n = value;                     // 3
    newNode->next = head;                   // 4

    return newNode;                         // 5
}

bool find(node *list, int valToFind)
// Check if a value exists in a LL
// Steps:
// 1. create a pointer, Traverse, to move through the list
// 2. If value matches, report success
// 3. If not a match, set Temp to next pointer in list and goto 2
// 4. If end of list, report failure
// takes big0(n) time
{
    node* traverse = list;                 // 1

    while (traverse != NULL)
    {
        if (traverse->n == valToFind)
        {
            return true;                    // 2
        }
        traverse = traverse->next;          // 3
    }

    return false;                           // 4
}

void printall(node *list)
// print each node, node's address, node's value, & next node's address in a LL
{
    int i = 0;
    while (list->next != NULL)
    {
        printf("node %i: %p, n: %i, next: %p\n", i, list, list->n, list->next);
        i++;
        list = list->next;
    }
}

void destroy(node *head)
// 1. (recursively) If you've reached a null pointer, stop
// 2. free the memory in the rest of the list
// 3. free the current node
{
    if (head == NULL)                   // 1
    {
        free(head);                     // 3
    }
    else
    {
        destroy(head->next);            // 2
        free(head);                     // 3
    }
}

void manual_testing()
{
    int num = 13; int num2 = 14; int num3 = 15;
    node *myNode = create(num);
    node *myNode2 = create(num2);
    node *myNode3 = create(num3);
    myNode -> next = myNode2;
    myNode2 -> next = myNode3;

    printf("num: %i\n", num);
    printf("myNode: %p\n", myNode);
    printf("myNode -> n: %i\n", (myNode -> n));
    printf("myNode -> next: %p\n", (myNode -> next));

    printf("num2: %i\n", num2);
    printf("myNode2: %p\n", myNode2);
    printf("myNode2 -> n: %i\n", (myNode2 -> n));
    printf("myNode2 -> next: %p\n", (myNode2 -> next));

    printf("num3: %i\n", num3);
    printf("myNode3: %p\n", myNode3);
    printf("myNode3 -> n: %i\n", (myNode3 -> n));
    printf("myNode3 -> next: %p\n", (myNode3 -> next));
}