#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
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
        if (!current->children[s[i] - 'A'])
            current->children[s[i] - 'A'] = createTree();
        current = current->children[s[i] - 'A'];
        i++;
    }

    if (!current->keyword)
        current->keyword = duplicate(s);
    if (!existsFile(current->first, file->name))
        current->first = insertSorted(current->first, file);
    return root;
}

Tree removeRecursive(Tree root, char *s, int index, int l) {
    if (root->children[s[index] - 'A'] && index < l)
            root->children[s[index] - 'A'] = removeRecursive(root->children[s[index] - 'A'], s, index + 1, l);

    int ok = 0;
    if (root->first)
        ok = 1;
    for (int i = 0; i < 58; i++)
            if (root->children[i])
                ok = 1;
    if (!ok) {
        free(root->keyword);
        free(root);
        root = NULL;
    }
    
    return root;
}

void deleteReferences(Tree root, char *keyword, char *name) {
    int i = 0;
    int l = strlen(keyword);

    Tree current = root;
    while (i < l) {
        current = current->children[keyword[i] - 'A'];
        i++;
    }

    current->first = removeFile(current->first, name);

    if (!current->first)
        removeRecursive(root, keyword, 0, strlen(keyword));
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

    for (int i = 0; i < 58; i++)
        if (root->children[i])
            printRecursive(fo, root->children[i]);
}

void listRecursive(Tree root, List *prefixFiles) {
    if (root->first) {
        List temp = root->first;
        while (temp) {
            if (!existsFile(*prefixFiles, ((File)temp->info)->name))
                *prefixFiles = insertSorted(*prefixFiles, temp->info);
            temp = temp->next;
        }
    }

    for (int i = 0; i < 58; i++)
        if (root->children[i])
            listRecursive(root->children[i], prefixFiles);
}

void freeTree(Tree root) {
    for (int i = 0; i < 58; i++)
        if(root->children[i])
            freeTree(root->children[i]);
    freeList(root->first);
    free(root->keyword);
    free(root);
}
