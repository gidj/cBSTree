#ifndef BTREE_H_
#define BTREE_H_ 

typedef struct Node *Node;

extern Node tree_create(int key, void* value, int elementSize, Node left, Node right);
extern void tree_destroy(Node tree); 

extern void* min_value(Node tree);
extern void* max_value(Node tree);
extern void* tree_search(Node tree, int key);

extern void tree_insert(Node tree, int key, void* value, int elementSize);
extern void tree_delete(Node *tree, int key); 

extern int  node_key(Node node);
extern void* node_value(Node node);
extern int  node_elementSize(Node node);
extern Node node_left(Node node);
extern Node node_right(Node node);

extern void set_key(Node node, int key);
extern void set_value(Node node, void* value, int elementSize);
extern void set_elementSize(Node node, int size);
extern void set_left(Node node, Node child);
extern void set_right(Node node, Node child);

extern Node* min_node(Node *tree);
extern Node* max_node(Node *tree);
extern void node_delete(Node *node);

extern void* node_data(Node node);
#endif
