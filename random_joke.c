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

int random_joke_index() {
    struct dirent* dir_entry;
    DIR* dir = opendir(JOKES_DIR);
    int count = 0;

    if (dir == NULL) {
        fprintf(stderr, "Unable to open the jokes directory");
        return -1;
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

    // Return error if no joke files found
    if (count == 0) {
        return -1;
    }

    // Random number between 1 and number of files
    srand(time(NULL));
    int random = rand() % count + 1;

    return random;
}

int random_joke(FILE* socket_pointer) {
    struct dirent* dir_entry;
    DIR* dir = opendir(JOKES_DIR);
    if (dir == NULL) {
        fputs("Error: Unable to open the jokes directory\n", socket_pointer);
        return 1;
    }

    int random = random_joke_index();
    if (random < 0) {
        fputs("Error: Unable to get a random index in the jokes directory\n", socket_pointer);
        return 1;
    }
    int index = 1;

    // Print the file at that index
    while ((dir_entry = readdir(dir)) != NULL) {
        if (strcmp(dir_entry->d_name,".") == 0 || 
            strcmp(dir_entry->d_name,"..") == 0
        ) {
            continue;
        }
        if (index == random) {
            char* filename = dir_entry->d_name;
            char pathname[strlen(JOKES_DIR) + strlen(filename) + 1];
            strcpy(pathname, JOKES_DIR);
            strcat(pathname, filename);
            FILE* joke = fopen(pathname, "r");
            if (joke == NULL) {
                fputs("Unable to open file", socket_pointer);
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