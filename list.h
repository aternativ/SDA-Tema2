#ifndef LIST_H
#define LIST_H

typedef struct list {
    void *info;
    struct list *next;
} *List, ListNode;

List insertFront(List head, void *info);
List insertSorted(List head, void *info);
int existsFile(List head, char *name);
List removeFile(List head, char *name);

#endif