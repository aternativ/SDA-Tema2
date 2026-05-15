#ifndef MULTITREE_H
#define MULTITREE_H

typedef struct list ListNode;
typedef struct file *File;

typedef struct tree {
    ListNode *first;
    struct tree *children[27];
} *Tree;

Tree addMultitree(Tree root, char *s, File file);
Tree removeRecursive(Tree root, char *s, int index, int l);

#endif