#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "helper.h"
#include "multitree.h"

Tree createTree() {
    Tree root = calloc(1, sizeof(struct tree));
    return root;
}

Tree addMultitree(Tree root, char *s, File file) {
    int i = 0;
    int l = strlen(s);

    if (!root)
        root = createTree();

    Tree current = root;
    while (i < l) {
        if (!current->children[s[i] - '0'])
            current->children[s[i] - '0'] = createTree();
        current = current->children[s[i] - '0'];
        i++;
    }

    current->first = insertSorted(current->first, file);
    return root;
}

Tree removeRecursive(Tree root, char *s, int index, int l) {
    if (index == l - 1) {
        int ok = 1;
        for (int i = 0; i < 26; i++)
            if (root->children[i])
                ok = 0;
        if (!ok)
            root = NULL;
        return root;
    }

    if (!root->children[s[index] - '0'])
            root->children[s[index] - '0'] = removeRecursive(root->children[s[index] - '0'], s, index + 1, l);
    
    return root;
}

void deleteReferences(Tree root, char *keyword, char *name) {
    int i = 0;
    int l = strlen(name);

    if (!root)
        root = createTree();

    Tree current = root;
    while (i < l) {
        if (!current->children[name[i] - '0'])
            current->children[name[i] - '0'] = createTree();
        current = current->children[name[i] - '0'];
        i++;
    }

    current->first = removeFile(current->first, name);
    if (!current->first)
        root = removeRecursive(root, keyword, 0, strlen(keyword));
}
