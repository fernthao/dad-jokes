/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: get.c
 Date created: Nov 26, 2025
 Description: Print 1 random joke
*/
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "constants.h"

int random_joke(FILE* socket_pointer) {
    struct dirent* dir_entry;
    DIR* dir = opendir(JOKES_DIR);
    int count = 0;

    if (dir == NULL) {
        fprintf(stderr, "Unable to open the jokes directory");
        return 1;
    }

    // Count number of files
    while ((dir_entry = readdir(dir)) != NULL) {
        if (strcmp(dir_entry->d_name,".") == 0 || 
            strcmp(dir_entry->d_name,"..") == 0
        ) {
            continue;
        }
        count++;
    }
    closedir(dir);

    // Random number - index of joke file 
    srand(time(NULL));
    int random = rand() % count + 1;

    // Set up to find the file at that index
    int index = 1;
    dir = NULL;
    dir = opendir(JOKES_DIR);
    if (dir == NULL) {
        fprintf(stderr, "Unable to open the jokes directory");
        return 1;
    }

    // Print the file at that index
    while ((dir_entry = readdir(dir)) != NULL) {
        if (strcmp(dir_entry->d_name,".") == 0 || 
            strcmp(dir_entry->d_name,"..") == 0
        ) {
            continue;
        }
        if (index == random) {
            char* filename = dir_entry->d_name;
            char pathname[strlen(JOKES_DIR)+1];
            strcpy(pathname, JOKES_DIR);
            strcat(pathname, filename);
            FILE* joke = fopen(pathname, "r");
            if (joke == NULL) {
                fprintf(stderr, "Unable to open file");
                return 1;
            }
            char buffer[JOKE_LINE_LENGTH];
            while (fgets(buffer, JOKE_LINE_LENGTH, joke) != NULL) {
                fputs(buffer, socket_pointer);
                fputs("\n", socket_pointer);
            };
            break;
        }
        index++;
    }

    closedir(dir);

    return 0;
}