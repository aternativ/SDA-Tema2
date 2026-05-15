#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"
#include "list.h"

List createNode (void *info) {
    List node = malloc(sizeof(ListNode));
    return node;
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
    char *s = head->info;
    char *p = info;
    if (strcmp(s, p) > 0)
        return insertFront(head, info);

    List current = head;
    while (current->next) {
        s = current->next->info;
        if (strcmp(s, p) > 0)
            break;
        current = current->next;
    }

    List new = createNode(info);
    new->next = current->next;
    current->next = new;
    return head;
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
