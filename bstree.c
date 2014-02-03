#include "bstree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

static void node_free(Node *n, void (*free_fn)(const void *value));
static void subtree_free(Node *subtree, void (*free_fn)(const void *data));
static void subtree_traverse(Node n, void (*fn)(const void* data));

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
  subtree_free(&(*tree)->root, (*tree)->free_fn);
}

static void subtree_free(Node *subtree, void (*free_fn)(const void *data))
{
  if (*subtree)
  {
    subtree_free(&(*subtree)->left, free_fn);
    subtree_free(&(*subtree)->right, free_fn);
    node_free(subtree, free_fn);

    *subtree = NULL;
  }
}


/* This helper function, node_new(), returns a Node object initialized with 
 * all of the values provided to it. 
 *
 * TODO: implement a cleaner memory interface with error handling. */ 

static Node node_new(const void* value, int elementSize, Node left, Node right)
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

static void node_remove(Node *n, void (*free_fn)(const void *value))
{
  if (*n)
  {
    Node *cursor = n;
 /* If the referenced node has no children, we simply free it and return. */ 
    if (!(*cursor)->left && !(*cursor)->right)
    {
      node_free(cursor, free_fn);
      return;
    }
/* If the referenced node has just one child node, we move that child node 
 * into the position of the node to be removed. */ 
    else if (!(*cursor)->left)
    {
      Node tmp = *cursor;
      *cursor = (*cursor)->right;
      node_free(&tmp, free_fn);
      return;
    }
    else if (!(*cursor)->right)
    {
      Node tmp = *cursor;
      *cursor = (*cursor)->left;
      node_free(&tmp, free_fn);
      return;
    }
/* Finally, if the node to be removed has two children, 'cursor' becomes reference
 * to the successor of the node to be removed via a while loop. We exploit the 
 * inherent structure of the search tree, which dictates that it must be the 
 * left-most node in the right subtree of the node to be removed. */ 
    else
    {
      cursor = &((*n)->right);
      while((*cursor)->left) {
        *cursor = (*cursor)->left;
      }

/* If the successor is in fact the right child of the node to be removed, our
 * job is easy and we just move the successor into place and attach the left 
 * child of the node to be removed to the left pointer of the successor node. 
 * Freeing the node to be removed completes the action.*/ 
      if ((*n)->right == (*cursor))
      {
        Node *tmp = n;
        (*cursor)->left = (*n)->left;
        *n = *cursor;

        node_free(tmp, free_fn);
        return;
      }
/* If the successor is further down the tree, we must do a bit more further
 * down the tree to keep it consistent.  */ 
      else
      {
        Node *tmp = n; // set tmp to reference the node to be deleted 
        (*cursor)->left = (*n)->left; // set the left child of the successor 
        *n = *cursor; // replace the node to be deleted with its successor 
        *cursor = (*cursor)->right; // set the LEFT child pointer of successor's parent to successor's left child 
        (*n)->right = (*tmp)->right; // set right child of the successor in its new place 

        node_free(tmp, free_fn);
        return;
      }
    }
  }
}

void bstree_traverse(BSTree tree, void (*fn)(const void* data))
{
  assert(tree && fn);
  if (tree->root)
  {
    subtree_traverse(tree->root, fn);
  }
} 

static void subtree_traverse(Node n, void (*fn)(const void* data))
{
  if (n)
  {
    fn(n->data);
    subtree_traverse(n->left, fn);
    subtree_traverse(n->right, fn);
  }
}

void* bstree_insert(BSTree t, void* value)
{
  assert(t && value);
  Node *cursor = &( t->root );

  if (*cursor)
  {
    while(*cursor) {
      switch ((t->cmp)(value, (*cursor)->data))
      {
        case 1: 
          *cursor = (*cursor)->right;
          break;
        case 0:
          return (*cursor)->data;
        case -1:
          *cursor = (*cursor)->left;
          break;
      } 
    }
    *cursor = node_new(value, t->elementSize, NULL, NULL);
  }
  else
  {
    *cursor = node_new(value, t->elementSize, NULL, NULL);
  }

  return (*cursor)->data;
}

void* bstree_search(BSTree t, void* value)
{
  assert(t);
  if (value)
  {
    Node *cursor = &( t->root );
    while(*cursor) {
      switch ((t->cmp)(value, (*cursor)->data))
      {
        case 1: 
          *cursor = (*cursor)->right;
          break;
        case 0:
          return (*cursor)->data;
        case -1:
          *cursor = (*cursor)->left;
          break;
      } 
    }
  }
  return NULL;
}

void* bstree_remove(BSTree t, void* value)
{
  assert(t);

  Node *cursor = &( t->root );
  while(*cursor) {
    switch ((t->cmp)(value, (*cursor)->data))
    {
      case 1: 
        *cursor = (*cursor)->right;
        break;
      case 0:
        void* data = malloc(t->elementSize);
        memcpy(data, (*cursor)->data, t->elementSize);
        node_free(cursor, t->free_fn);
        return data;
      case -1:
        *cursor = (*cursor)->left;
        break;
    } 
  }
  return NULL;
}

