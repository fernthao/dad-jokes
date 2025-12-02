/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: edit.c
 Date created: Dec 1, 2025
 Description: Edit the joke with given title
*/
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

int edit(char* joke_title, char* edited_joke) {
    // Construct the joke file path
    char filepath[FILE_PATH_LEN];
    snprintf(filepath, FILE_PATH_LEN, "%s%s", JOKES_DIR, joke_title);

    // Check if exist
    FILE * fp = fopen(filepath, "r");
    if ( fp == NULL) {
        fprintf(stderr, "Joke title does not exist");
        return 1;
    }
    fclose(fp);

    // Edit file
    fp = fopen(filepath, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error creating joke file");
        return 1;
    }
    
    if (fprintf(fp, "%s", edited_joke) < 0) {
        fprintf(stderr, "Error editing joke file");
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
