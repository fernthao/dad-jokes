/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: create.c
 Date created: Dec 1, 2025
 Description: Add a joke to the database
 Returns -1 if error, 0 if successful
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "constants.h"
#include "write_to_file.h"

/* write the joke to a file in the jokes dir, and write success or error message to the client socket csp*/
int create(char* joke_title, char* joke, FILE* csp) {
    if (joke_title == NULL || joke == NULL) {
        fputs("Error: null joke title or joke content\n", csp);
        return ERROR;
    }
    
    // Create jokes directory if it doesn't exist
    struct stat st = {0};
    if (stat(JOKES_DIR, &st) == -1) {
        if (mkdir(JOKES_DIR, 0755) != 0) {
            fputs("Error: Could not create jokes directory\n", csp);
            return ERROR;
        }
    }
    
    char filepath[FILE_PATH_LEN];
    snprintf(filepath, FILE_PATH_LEN, "%s%s", JOKES_DIR, joke_title);
    
    // Check if exist
    FILE * fp = fopen(filepath, "r");
    if ( fp != NULL) {
        fputs("Error: Joke title already exist, pick a different title\n", csp);
        fclose(fp);
        return ERROR;
    }
    if (write_to_file(joke_title, joke, csp, filepath) == 0) {
        fputs("Joke added!\n", csp);
        return 0;
    } else return ERROR;
}