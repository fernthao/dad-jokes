/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: edit.c
 Date created: Dec 1, 2025
 Description: Edit the joke with given title
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "write_to_file.h"

int edit(char* joke_title, char* edited_joke, FILE* csp) {
    if (joke_title == NULL || edited_joke == NULL) {
        fputs("Error: null joke title or joke content\n", csp);
        return ERROR;
    }
    
    // Construct the joke file path
    char filepath[FILE_PATH_LEN];
    snprintf(filepath, FILE_PATH_LEN, "%s%s", JOKES_DIR, joke_title);

    // Check if exist
    FILE * fp = fopen(filepath, "r");
    if ( fp == NULL) {
        fputs("Error: Joke title does not exist\n", csp);
        return ERROR;
    }
    fclose(fp);

    if (write_to_file(joke_title, edited_joke, csp, filepath) == 0) {
        fputs("Joke edited!\n", csp);
        return 0;
    } else return ERROR;
}
