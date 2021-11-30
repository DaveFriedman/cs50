#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int n;
    struct node *left;
    struct node *right;
}
node;


node* create(int value);                // create a new tree
void  insert(node **root, int value);    // insert a node into a tree
bool  find(node *root, int value);      // check if a value exists in a tree
void  printall(node *root);             // print all vals in a tree
void  destroy(node *root);              // delete an entire tree
//void  destroynode(node *root);          // delete a single node

int main()
{
    int number = 10;
    node *myNode = create(number);
    for (int i = 0; i < 8; i++)
    {
        number += i;
        printf("%i, ", number);
        insert(&myNode, number);
//        printall(myNode);
        printf("\n");

    }

    printall(myNode);
    destroy(myNode);

    return 0;
}

node* create(int value)
// create a new tree
// Steps:
// 1. allocate space for new node
// 2. check to avoid segfault; don't run out of memory
// 3. initialize Val field
// 4. initialize Left and Right fields
// 5. return a Pointer to the new node
{
    node *newNode = malloc(sizeof(node));       // 1

    if (newNode == NULL)                        // 2
    {
        printf("MALLOC FAIL\n");
    }

    newNode->n     = value;                     // 3
    newNode->left  = NULL;                      // 4
    newNode->right = NULL;                      // 4

    return newNode;                             // 5
}

/*
// Because insert() is Void and does not return a value, it must take a
// pointer to a pointer to a node (**root, rather than *root)

void insert(node **root, int value) // pointing to a pointer fuckery
// Insert a value into a binary tree
{
    if ((*root)->left == NULL || (*root)->right == NULL)
    {
        node *newNode = create(value);
        if ((*root)->n < value)
        {
             (*root)->left = newNode;
             printf("new L: %p, ", (*root)->left);
        }
        if ((*root)->n > value)
        {
            (*root)->right = newNode;
            printf("new R: %p, ", (*root)->right);
        }
        printf("newNode %p, L %p, R %p, n %i, root %p ", newNode, (*root)->left, (*root)->right, newNode->n, root);
    }
    else if (value < (*root)->n)
    {
        printf("left, ");
        insert(&(*root)->left, value); // value of the pointer's address fuckery
    }
    else if (value > (*root)->n)
    {
        printf("right, ");
        insert(&(*root)->right, value); // value of the pointer's address fuckery
    }
    else if (value == (*root)->n)
    {
        return;
    }
}
*/

bool find(node *root, int valToFind)
// Check if a value exists in a tree
// Steps:
// 0. Assume tree is sorted, and compare each leaf to valToFind
// 1. If leaf == NULL, end search thread, return False
// 2. If valToFind < n, go Left
// 3. If valToFind > n, go Right
// 4. If valToFind == n, return True

{
    if (root == NULL)                           // 1
    {
        return false;
    }
    else if (valToFind < root->n)               // 2
    {
        return find(root->left, valToFind);
    }
    else if (valToFind > root->n)               // 3
    {
        return find(root->right, valToFind);
    }
    else if (valToFind == root->n)              // 4
    {
        return true;
    }
    return root;
}

void printall(node *root)
// print each node's address, value, and left and right addresses
{
    if (root != NULL)
    {
        printf("node: %p, n: %i, left: %p, right: %p\n", root, root->n, root->left, root->right);
    }
    if (root->left != NULL)
    {
        printall(root->left);
    }
    if (root->right != NULL)
    {
        printall(root->right);
    }
}

void printAllInOrder(node *root)
// print each node's address, value and left and right addresses,
// in order from leftmost to rightmost node
{
    if (root == NULL)
    {
        return;
    }

    printAllInOrder(root->left);

    printf("node: %p, n: %i, left: %p, right: %p\n", root, root->n, root->left, root->right);

    printAllInOrder(root->right);
}

void destroy(node *root)
// 1. (recursively) if you've reached a null pointer, stop
// 2. free the memory in the rest of the tree
// 3. free the current node
{
    if (root == NULL)
    {
        free(root);
    }
    else
    {
        destroy(root->left);
        destroy(root->right);

        free(root);
    }
}