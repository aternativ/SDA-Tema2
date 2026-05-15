#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "list.h"
#include "word.h"
#include "helper.h"
#include "multitree.h"

void ignoreInput(System system) {
    FILE *fi = system->fi;
    int c;
    while ((c = fgetc(fi) != '\n' && c != EOF));
}

// 2.1
//

void add(System system) {
    int n_keywords;
    // get rid of the first whitespace
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    fgetc(fi);

    File file = malloc(sizeof(struct file));
    file->name = readWord(system);
    fscanf(fi, "%d", &file->score);
    // fscanf leaves the whitespace in the buffer
    fscanf(fi, "%d", &n_keywords);
    // so we consume it
    fgetc(fi);

    if (existsFile(system->files, file->name)) {
        fprintf(fo, "EXISTS\n");
        ignoreInput(system);
        free(file->name);
        free(file);
        return;
    }

    Tree root = system->root;
    for (int i = 1; i < n_keywords; i++) {
        char *keyword = readWord(system);
        insertFront(file->keywords, keyword);
        addMultitree(root, keyword, file);
    }
    insertFront(system->files, file);

    fprintf(fo, "OK\n");
}

// 2.2
//

void delete(System system) {
    // get rid of the first whitespace
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    fgetc(fi);

    char *name = readWord(system);
    // todo find file, delete its keywords
    List files = system->files;
    // edge case - is head
    if (strcmp(((File)files->info)->name, name) == 0) {
        List temp = files;
        system->files = files->next;
        free(temp);
    }

    while (files->next && strcmp(((File)files->info)->name, name) != 0)
        files = files->next;

    if (!files->next) {
        fprintf(fo, "NOT FOUND\n");
        free(name);
        return;
    }

    // delete keywords from next and then delete next
    List it = ((File)files->next->info)->keywords;

    while (it) {
        removeRecursive(system->root, (char *)it->info, 0, strlen((char *)it->info));
        List node = it;
        it = it->next;
        free(node);
    }

    List temp = files->next;
    files->next = files->next->next;
    free(temp);
}

// 2.3
//

void addKW(System system) {
    // get rid of the first whitespace
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    fgetc(fi);

    char *name = readWord(system);
    List current = system->files;

    char *keyword = readWord(system);
    while (current && strcmp((char *)current->info, keyword) != 0)
        current = current->next;

    if (!current) {
        fprintf(fo, "NOT FOUND\n");
        free(name);
        free(keyword);
        return;
    }

    // found:
    List temp = ((File)current->info)->keywords;
    while (temp) {
        if (strcmp(temp->info, keyword) == 0) {
            fprintf(fo, "OK\n");
            free(name);
            free(keyword);
            return;
        }
        temp = temp->next;
    }

    // not in keywords, add
    insertFront(((File)current->info)->keywords, keyword);
    addMultitree(system->root, keyword, current->info);

    free(name);
}

//
//

