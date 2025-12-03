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
#include <sys/stat.h>
#include <sys/types.h>
#include "constants.h"

int create(char* joke_title, char* joke, FILE* csp) {
    if (joke_title == NULL || joke == NULL) {
        fputs("Error: null joke title or joke content\n", csp);
        return 1;
    }
    
    // Create jokes directory if it doesn't exist
    struct stat st = {0};
    if (stat(JOKES_DIR, &st) == -1) {
        if (mkdir(JOKES_DIR, 0755) != 0) {
            fputs("Error: Could not create jokes directory\n", csp);
            return 1;
        }
    }
    
    char filepath[FILE_PATH_LEN];
    snprintf(filepath, FILE_PATH_LEN, "%s%s", JOKES_DIR, joke_title);
    
    // Check if exist
    FILE * fp = fopen(filepath, "r");
    if ( fp != NULL) {
        fputs("Error: Joke title already exist, pick a different title\n", csp);
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
                char err[ERR_MSG_LEN];
                snprintf(err, ERR_MSG_LEN, "Line %d exceeds maximum length of %d characters\n", line_count + 1, JOKE_LINE_LENGTH);
                fputs(err, csp);
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
        char err[ERR_MSG_LEN];
        snprintf(err, ERR_MSG_LEN, "Joke exceeds maximum of %d lines (has %d lines)\n", MAX_LINES, line_count);
        fputs(err, csp);
        return 1;
    }

    // Create file under ./jokes with the joke_title as the name
    fp = fopen(filepath, "w");
    if (fp == NULL) {
        fputs("Error creating joke file\n", csp);
        return 1;
    }
    
    // Write the joke content to the file
    if (fprintf(fp, "%s", joke) < 0) {
        fputs("Error writing to joke file\n", csp);
        fclose(fp);
        return 1;
    }
    
    // Close the file
    if (fclose(fp) != 0) {
        fputs("Error closing joke file\n", csp);
        return 1;
    }
    
    fputs("Joke added!\n", csp);
    return 0;
}