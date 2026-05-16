#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "list.h"
#include "word.h"
#include "helper.h"
#include "multitree.h"
#include "heap.h"

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
    // fgetc(fi);

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

    for (int i = 1; i <= n_keywords; i++) {
        char *keyword = readWord(system);
        file->keywords = insertFront(file->keywords, keyword);
        system->root = addMultitree(system->root, keyword, file);
    }
    system->files = insertFront(system->files, file);

    fprintf(fo, "OK\n");
}

// 2.2
//

void delete(System system) {
    // get rid of the first whitespace
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    // fgetc(fi);

    char *name = readWord(system);
    // todo find file, delete its keywords
    List files = system->files;
    // edge case - is head
    if (strcmp(((File)files->info)->name, name) == 0) {
        List temp = files;
        system->files = files->next;

        File found = ((File)files->info);
        while (found->keywords) {
            // removeRecursive(system->root, (char *)it->info, 0, strlen((char *)it->info));
            deleteReferences(system->root, found->keywords->info, name);
            List node = found->keywords;
            found->keywords = found->keywords->next;
            free(node);
        }

        // what to do with freeFile??
        freeFile(temp->info);
        free(temp);
        fprintf(fo, "OK\n");
        return;
    }

    while (files->next && strcmp(((File)files->next->info)->name, name) != 0)
        files = files->next;

    if (!files->next) {
        fprintf(fo, "NOT FOUND\n");
        free(name);
        return;
    }

    // delete keywords from next and then delete next
    File found = ((File)files->next->info);

    while (found->keywords) {
        // removeRecursive(system->root, (char *)it->info, 0, strlen((char *)it->info));
        deleteReferences(system->root, found->keywords->info, name);
        List node = found->keywords;
        found->keywords = found->keywords->next;
        free(node);
    }

    List temp = files->next;
    files->next = files->next->next;
    freeFile(temp->info);
    free(temp);
    fprintf(fo, "OK\n");
}

// 2.3
//

void addKW(System system) {
    // get rid of the first whitespace
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    // fgetc(fi);

    char *name = readWord(system);
    List current = system->files;

    
    while (current && strcmp(((File)current->info)->name, name) != 0)
        current = current->next;

    if (!current) {
        fprintf(fo, "NOT FOUND\n");
        free(name);
        ignoreInput(system);
        return;
    }

    // found:
    char *keyword = readWord(system);
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
    ((File)current->info)->keywords = insertFront(((File)current->info)->keywords, keyword);
    addMultitree(system->root, keyword, current->info);

    free(name);
    fprintf(fo, "OK\n");
}

// 2.4
//

void delKW(System system) {
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    // fgetc(fi);

    char *name = readWord(system);
    List current = system->files;

    while (current && strcmp(((File)current->info)->name, name) != 0)
        current = current->next;

    if (!current) {
        fprintf(fo, "NOT FOUND\n");
        free(name);
        return;
    }

    // found file, search for keyword:
    char *keyword = readWord(system);
    List keywords = ((File)current->info)->keywords;
    // edge case - is head
    if (strcmp(keywords->info, keyword) == 0) {
        List temp = keywords;
        ((File)current->info)->keywords = keywords->next;
        deleteReferences(system->root, keyword, name);
        if (!((File)current->info)->keywords)
            removeFile(system->files, name);
        free(temp->info);
        free(temp);
        fprintf(fo, "OK\n");
        return;
    }

    while (keywords->next && strcmp(keywords->next->info, keyword) != 0)
        keywords = keywords->next;
    
    if (!keywords->next)  {
        // the keyword doesn't exist
        free(name);
        free(keyword);
        fprintf(fo, "OK\n");
        return;
    }

    // we found the keyword, remove it
    List temp = keywords->next;
    keywords->next = keywords->next->next;
    deleteReferences(system->root, keyword, name);
    free(temp->info);
    free(temp);

    free(name);
    free(keyword);
    fprintf(fo, "OK\n");
}

// 2.5
//

void find(System system) {
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    // fgetc(fi);
    Tree root = system->root;

    char *keyword = readWord(system);
    int i = 0;
    int l = strlen(keyword);

    Tree current = root;
    while (i < l) {
        if (current->children[keyword[i] - 'A'])
            current = current->children[keyword[i] - 'A'];
        else {
            fprintf(fo, "EMPTY\n");
            free(keyword);
            return;
        }
        i++;
    }

    int listLength = length(current->first);
    if (!listLength) {
        fprintf(fo, "EMPTY\n");
        free(keyword);
        return;
    }

    fprintf(fo, "%d ", listLength);
    List files = current->first;
    while (files) {
        fprintf(fo, "%s ", ((File)files->info)->name);
        files = files->next;
    }
    fprintf(fo, "\n");
    free(keyword);
}

// 2.6
//

void topK(System system) {
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    // fgetc(fi);
    Tree root = system->root;

    int k;
    char *keyword = readWord(system);
    fscanf(fi, "%d", &k);
    // get rid of '\n'
    fgetc(fi);
    int i = 0;
    int l = strlen(keyword);

    Tree current = root;
    while (i < l) {
        if (current->children[keyword[i] - 'A'])
            current = current->children[keyword[i] - 'A'];
        else {
            fprintf(fo, "EMPTY\n");
            free(keyword);
            return;
        }
        i++;
    }

    int listLength = length(current->first);
    if (!listLength) {
        fprintf(fo, "EMPTY\n");
        free(keyword);
        return;
    }

    fprintf(fo, "%d ", listLength < k ? listLength: k);
    List it = current->first;
    Heap h = initHeap();
    while (it) {
        insertHeap(h, it->info);
        it = it->next;
    }

    while (!isEmptyHeap(h) && k--) {
        File max = extractMax(h);
        fprintf(fo, "%s ", max->name);
    }
    fprintf(fo, "\n");

    freeHeap(h);
    free(keyword);
}

// 2.7
// 

void print(System system) {
    FILE *fi = system->fi;
    FILE *fo = system->fo;
    Tree root = system->root;

    int ok = 0;
    for (int i = 0; i < 58; i++)
        if (root->children[i]) {
            ok = 1;
        }

    if (!ok) {
        fprintf(fo, "EMPTY\n");
        return;
    }

    printRecursive(fo, root);

}
