/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: create.c
 Date created: Dec 1, 2025
 Description: Add a joke to the database
 Returns 1 if error, 0 if successful
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

int create(char* joke_title, char* joke) {
    char filepath[FILE_PATH_LEN];
    snprintf(filepath, FILE_PATH_LEN, "%s%s", JOKES_DIR, joke_title);
    
    // Check if exist
    FILE * fp = fopen(filepath, "r");
    if ( fp != NULL) {
        fprintf(stderr, "Joke title already exist, pick a different title");
        fclose(fp);
        return 1;
    }

    // Create file under ./jokes with the joke_title as the name
    fp = fopen(filepath, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error creating joke file");
        return 1;
    }
    
    // Write the joke content to the file
    if (fprintf(fp, "%s", joke) < 0) {
        fprintf(stderr, "Error writing to joke file");
        fclose(fp);
        return 1;
    }
    
    // Close the file
    if (fclose(fp) != 0) {
        fprintf(stderr, "Error closing joke file");
        return 1;
    }
    
    return 0;
}