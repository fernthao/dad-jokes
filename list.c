/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: list.c
 Date created: Nov 24, 2025
 Description: List the joke titles under /jokes
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "constants.h"

int list(FILE* socket_pointer) {
    struct dirent* dir_entry;
    DIR* dir = opendir("./jokes");

    if (dir == NULL) {
        fputs("Error: Empty jokes database. Add some jokes!", socket_pointer);
        return ERROR;
    }

    while ((dir_entry = readdir(dir)) != NULL) {
        if (strcmp(dir_entry->d_name,".") == 0 || 
            strcmp(dir_entry->d_name,"..") == 0
        ) {
            continue;
        }
        fputs(dir_entry->d_name, socket_pointer);
        fputs("\n", socket_pointer);
    }
    closedir(dir);
    return 0;
}