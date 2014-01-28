#include "newtree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void node_free();

typedef struct Node *Node;
struct Node {
  void* data;
  Node left, right;
};

struct BSTree {
  Node root;
  unsigned elementSize;
  int (*cmp)(const void* x, const void* y);
  void (*free_fn)(const void* value);
};

/* When creating a new bstree, the client must provide:
 * 1. the size of the object that the tree will hold
 * 2. a pointer to a function to be used to compare two different values
 * 3. a pointer to a function to free objects. 
 * If the pointer to the free_fn() function is NULL, objects will be freed simply 
 * using free() from the standard library. */ 

BSTree bstree_new(unsigned elementSize,
    int (*cmp)(const void* x, const void* y),
    void (*free_fn)(const void* value))
{
  assert(elementSize > 0);
  BSTree t;
  t->root = NULL;
  t->elementSize = elementSize;
  t->cmp = cmp;
  t->free_fn = free_fn;

  return t;
}

void bstree_free(BSTree *tree)
{

/* TODO: Make a helper function that recursively destroys subtrees */ 
  assert(*tree && tree);
  if ((*tree)->root)
  {
    Node left = (*tree)->root->left;
    Node right = (*tree)->root->right;

    node_free((*tree)->root, (*tree)->free_fn);

  }
}

/* This helper function, node_new(), returns a Node object initialized with 
 * all of the values provided to it. 
 *
 * TODO: implement a cleaner memory interface with error handling. */ 

Node node_new(const void* value, int elementSize, Node left, Node right)
{
  assert(value);
  Node n = malloc(sizeof(*n));
  if (n)
  {
    n->data = malloc(elementSize);
    if (n->data)
    {
      memcpy(n->data, value, elementSize);
    }
    else
    {
      printf("There was a memory error. Exiting...\n");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    printf("There was a memory error. Exiting...\n");
    exit(EXIT_FAILURE);
  }

  n->left = left;
  n->right = right;

  return n;
}

/* This helper function, node_free(), only frees the data of the node, and the node itself. 
 * Misuse can cause lost pointers--that's why it's a static function. */

static void node_free(Node *n, void (*free_fn)(const void *value))
{
  assert(*n && n);
  if (free_fn)
  {
    free_fn((*n)->data);
  }
  else
  {
    free((*n)->data);
  }

  free(*n);
  *n = NULL;
}

