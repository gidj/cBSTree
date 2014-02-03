#ifndef BSTREE_H_
#define BSTREE_H_ 

typedef struct BSTree *BSTree;

extern BSTree bstree_new(unsigned elementSize,
        int (*cmp)(const void* x, const void* y),
        void (*hash)(const void* value));
extern void bstree_free(BSTree *tree);
extern void bstree_traverse(BSTree tree, void (*fn)(const void* data));
extern void* bstree_insert(BSTree t, void* value);
extern void* bstree_search(BSTree t, void* value);
extern void* bstree_remove(BSTree t, void* value);

#endif
