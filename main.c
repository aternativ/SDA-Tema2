#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "helper.h"
#include "multitree.h"
#include "word.h"
#include "functions.h"
#include "system.h"

int main() 
{
    System system = calloc(1, sizeof(struct system));
    FILE *fi, *fo;
    system->fi = fi = fopen("indexare.in", "r");
    system->fo = fo = fopen("indexare.out", "w");

    if (!fi) {
        fclose(fo);
        return 0;
    }

    int q;
    fscanf(fi, "%d", &q);
    fgetc(fi);
    
    char *command;
    while (q--) {
        command = readWord(system);
        if (strcmp(command, "ADD") == 0)
            add(system);
        else if (strcmp(command, "DEL") == 0)
            delete(system);
        else if (strcmp(command, "ADDKW") == 0)
            addKW(system);
        else if (strcmp(command, "DELKW") == 0)
            delKW(system);
        else if (strcmp(command, "FIND") == 0)
            find(system);
        else if (strcmp(command, "TOPK") == 0)
            topK(system);
        else if (strcmp(command, "PRINT") == 0)
            print(system);
        else if (strcmp(command, "PREFIX") == 0)
            prefix(system);
        else
            fprintf(fo, "UNRECOGNIZED\n");

        free(command);
    }
    
    fclose(fi);
    fclose(fo);
    freeTree(system->root);
    List files = system->files;
    while (files) {
        List temp = files;
        free(((File)files->info)->name);
        List keywords = ((File)files->info)->keywords;
        while (keywords) {
            free(keywords->info);
            List tempKeyword = keywords;
            keywords = keywords->next;
            free(tempKeyword);
        }
        free(files->info);
        files = files->next;
        free(temp);
    }
    free(system);
    return 0;
}