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

    // Count the number of lines in the joke
    int line_count = 0;
    int current_line_length = 0;
    for (int i = 0; joke[i] != '\0'; i++) {
        if (joke[i] == '\n') {
            line_count++;
            current_line_length = 0;
        } else {
            current_line_length++;
            // Enforce JOKE_LINE_LENGTH constraint
            if (current_line_length > JOKE_LINE_LENGTH) {
                fprintf(stderr, "Line %d exceeds maximum length of %d characters", line_count + 1, JOKE_LINE_LENGTH);
                return 1;
            }
        }
    }
    // Count the last line if it doesn't end with newline
    if (strlen(joke) > 0 && joke[strlen(joke) - 1] != '\n') {
        line_count++;
    }
    
    // Enforce MAX_LINES constraint
    if (line_count > MAX_LINES) {
        fprintf(stderr, "Joke exceeds maximum of %d lines (has %d lines)", MAX_LINES, line_count);
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