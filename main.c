#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include "functions.h"
#include "system.h"

int main() 
{
    System system = malloc(sizeof(struct system));
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
        printf("Command: %s\n", command);
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
        else
            fprintf(fo, "UNRECOGNIZED\n");

        free(command);
    }
    
    fclose(fi);
    fclose(fo);
    return 0;
}