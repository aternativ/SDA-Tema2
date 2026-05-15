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

    current->keyword = s;
    current->first = insertSorted(current->first, file);
    return root;
}

Tree removeRecursive(Tree root, char *s, int index, int l) {
    if (root->children[s[index] - '0'] && index < l - 1)
            root->children[s[index] - '0'] = removeRecursive(root->children[s[index] - '0'], s, index + 1, l);

    int ok = 0;
    if (root->first)
        ok = 1;
    for (int i = 0; i < 26; i++)
            if (root->children[i])
                ok = 1;
    if (!ok)
            root = NULL;
    
    return root;
}

void deleteReferences(Tree root, char *keyword, char *name) {
    int i = 0;
    int l = strlen(keyword);

    Tree current = root;
    while (i < l) {
        current = current->children[keyword[i] - '0'];
        i++;
    }

    current->first = removeFile(current->first, name);
    if (!current->first)
        root = removeRecursive(root, keyword, 0, strlen(keyword));
}

void printRecursive(FILE *fo, Tree root) {
    if (root->first) {
        fprintf(fo, "%s ", root->keyword);
        int l = length(root->first);
        fprintf(fo, "%d ", l);
        List files = root->first;

        while (files) {
            fprintf(fo, "%s ", ((File)files->info)->name);
            files = files->next;
        }
        fprintf(fo, "\n");
    }

    for (int i = 0; i < 26; i++)
        if (root->children[i])
            printRecursive(fo, root->children[i]);
}
