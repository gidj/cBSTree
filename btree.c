#include "btree.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Node {
  int key;
  void* value;
  int elementSize;
  Node left, right;
};

Node tree_create(int key, void* value, int elementSize, Node left, Node right)
{
  assert(value);
  assert(elementSize > 0);
  
  Node node = malloc(sizeof(*node));
  assert(node);

  set_key(node, key);
  set_value(node, value, elementSize);
  set_elementSize(node, elementSize);
  set_left(node, left);
  set_right(node, right);

  return node;
}

void tree_destroy(Node tree)
{
  assert(tree);
  Node left = tree->left;
  Node right = tree-> right;

  free(tree->value);
  free(tree);

  if (left) { tree_destroy(left); }
  if (right) { tree_destroy(right); }
}

void* min_value(Node tree)
{
  assert(tree);
  Node current = tree;
  
  while(current->left) {
    current = current->left;
  }
  return current->value;
}

void* max_value(Node tree)
{
  assert(tree);
  Node current = tree;
  
  while(current->right) {
    current = current->right;
  }
  return current->value;
}

void* tree_search(Node tree, int key)
{
  assert(tree);
  Node current = tree;

  do {
    if      (key == current->key) { return current->value; }
    else if (key < current->key) { current = current->left; } 
    else if (key > current->key) { current = current->right; }
  } while(current); 

  return NULL;
}

void tree_insert(Node tree, int key, void* value, int elementSize)
{
  assert(tree);
  Node current = tree;

  if (key == current->key)
  {
    set_value(current, value, elementSize);
    set_elementSize(current, elementSize);
    return;
  } 
  else if (key < current->key)
  { 
    if (current->left)
    {
      tree_insert(current->left, key, value, elementSize);
    } 
    else
    {
      set_left(current, tree_create(key, value, elementSize, NULL, NULL));
      return;
    }
  } 
  else if (key > current->key)
  {
    if (current->right)
    {
      tree_insert(current->right, key, value, elementSize);
    }
    else
    {
      set_right(current, tree_create(key, value, elementSize, NULL, NULL));
      return;
    }
  }
}

void tree_delete(Node *tree, int key)
{
  if (*tree)
  {
    if (key == (*tree)->key)
    {
      node_delete(tree);
    }
    else if (key < (*tree)->key)
    {
      tree_delete(&(*tree)->left, key);
    }
    else if (key > (*tree)->key)
    {
      tree_delete(&(*tree)->right, key);
    }
  }
  return;
}

void node_delete(Node *node)
{
  if (*node)
  {
    Node current = *node;
    
    if (!node_left(*node) && !node_right(*node))
    {
      Node tmp = *node;
      *node = NULL;
      free(node_value(tmp));
      free(tmp);
      return;
    }
    else if (!node_left(*node))
    {
      Node tmp = *node;
      *node = node_right(*node);
      free(tmp->value);
      free(tmp);
      return;
    }
    else if (!node_right(*node))
    {
      Node tmp = *node;
      *node = node_left(*node);
      free(tmp->value);
      free(tmp);
      return;
    }
    else
    {
      Node successor = node_right((*node));
      Node successorParent = (*node);

      while(successor->left) {
        successorParent = successor;
        successor = node_left(successor);
      }

      if (node_right(*node) == successor)
      {
        Node temp = *node;
        set_left(successor, node_left(*node));
        *node = successor;

        free(temp->value);
        free(temp);
        return;
      }
      else
      {
        set_left(successorParent, node_right(successor));
        set_left(successor, node_left(*node));
        set_right(successor, node_right(*node));

        Node temp = *node;
        *node = successor;
        
        free(temp->value);
        free(temp);

        /*set_key(*node, successor->key);*/
        /*set_elementSize(*node, successor->elementSize);*/
        /*set_value(*node, node_value(successor), node_elementSize(successor));*/

        /*free(successor->value);*/
        /*free(successor);*/

        return;
      }
    }
  }
}

/* Helper functions that get and set values in Node types.*/ 

int node_key(Node node)
{ return node->key; }

void* node_value(Node node)
{ return node->value; }

int  node_elementSize(Node node)
{ return node->elementSize; }

Node node_left(Node node)
{ return node->left; }

Node node_right(Node node)
{ return node->right; }

void set_key(Node node, int key)
{ node->key = key; }

void set_value(Node node, void* value, int elementSize)
{ 
  void* tmp = realloc(node->value, elementSize);

  if (tmp) 
  { 
    node->value = tmp; 
  }
  else 
  { 
    printf("Memory allocation failed. Exiting...\n");
    exit(EXIT_FAILURE); 
  }

  memcpy(node->value, value, elementSize);
}

void set_elementSize(Node node, int size)
{ node->elementSize = size; }

void set_left(Node node, Node child)
{ node->left = child; }

void set_right(Node node, Node child)
{ node->right = child; }

Node* min_node(Node* tree)
{
  assert(tree);
  assert(*tree);
  
  while((*tree)->left) {
    tree = &(*tree)->left;
  }
  return tree;
}

Node* max_node(Node* tree)
{
  assert(tree);
  assert(*tree);

  while((*tree)->right) {
    tree = &(*tree)->right;
  }
  return tree;
}

void* node_data(Node node)
{
  return node->value;
}
