#include "newtree.h"
#include <assert.h>
#include <stdlib.h>

typedef struct Node *Node;

struct Node {
  void* key;
  void* value;
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
  assert(*tree && tree);
  if ((*tree)->root)
  {
    
  }

}
