#include "newtree.h"
#include <assert.h>
#include <stdlib.h>

typedef struct Node *Node;

struct Node {
  void* data;
  Node left, right;
};

struct BSTree {
  Node root;
  unsigned elementSize;
  int (*cmp)(const void* x, const void* y);
  void (*free)(const void* value);
};

extern BSTree bstree_new(unsigned elementSize,
    int (*cmp)(const void* x, const void* y),
    void (*free)(const void* value))
{
  assert(elementSize > 0);
  BSTree t;
  t->root = NULL;
  t->elementSize = elementSize;
  t->cmp = cmp;
  t->free = free;

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

    node_free((*tree)->root);


  }
  
}
