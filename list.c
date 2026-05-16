#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "list.h"

List createNode(void *info) {
    List node = malloc(sizeof(ListNode));
    node->info = info;
    return node;
}

int length(List head) {
    int l = 0;
    while (head) {
        l++;
        head = head->next;
    }
    return l;
}

List insertFront(List head, void *info) {
    List newNode = createNode(info);
    newNode->next = head;
    return newNode;
}

List insertSorted(List head, void *info) {
    // check for edge case
    if (!head)
        return insertFront(head, info);
    char *s = ((File)head->info)->name;
    char *p = ((File)info)->name;
    if (strcmp(s, p) > 0)
        return insertFront(head, info);

    List current = head;
    while (current->next) {
        s = ((File)current->next->info)->name;
        if (strcmp(s, p) > 0)
            break;
        current = current->next;
    }

    List new = createNode(info);
    new->next = current->next;
    current->next = new;
    return head;
}

void freeFile(File file) {
    free(file->keywords);
    free(file->name);
    free(file);
}

List removeFile(List head, char *name) {
    List it = head;
    if (strcmp(((File)head->info)->name, name) == 0) {
        head = head->next;
        free(it);
        return head;
    }

    while(it->next && strcmp(((File)it->next->info)->name, name) != 0)
        it = it->next;

    List temp = it->next;
    it->next = it->next->next;
    free(temp);
    
    return head;
}

int existsFile(List head, char *name) {
    File current;
    while (head) {
        current = head->info;
        if (strcmp(current->name, name) == 0)
            return 1;
        head = head->next;
    }

    return 0;
}

void freeList(List head) {
    while (head) {
        List temp = head;
        head = head->next;
        free(temp);
    }
}
