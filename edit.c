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

int edit(char* joke_title, char* edited_joke, FILE* csp) {
    if (joke_title == NULL || edited_joke == NULL) {
        fputs("Error: null joke title or joke content\n", csp);
        return 1;
    }
    
    // Construct the joke file path
    char filepath[FILE_PATH_LEN];
    snprintf(filepath, FILE_PATH_LEN, "%s%s", JOKES_DIR, joke_title);

    // Check if exist
    FILE * fp = fopen(filepath, "r");
    if ( fp == NULL) {
        fputs("Error: Joke title does not exist\n", csp);
        return 1;
    }
    fclose(fp);

    // Count the number of lines in the joke
    int line_count = 0;
    int current_line_length = 0;
    for (int i = 0; edited_joke[i] != '\0'; i++) {
        if (edited_joke[i] == '\n') {
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
    if (strlen(edited_joke) > 0 && edited_joke[strlen(edited_joke) - 1] != '\n') {
        line_count++;
    }
    
    // Enforce MAX_LINES constraint
    if (line_count > MAX_LINES) {
        char err[ERR_MSG_LEN];
        snprintf(err, ERR_MSG_LEN, "Joke exceeds maximum of %d lines (has %d lines)\n", MAX_LINES, line_count);
        fputs(err, csp);
        return 1;
    }

    // Edit file
    fp = fopen(filepath, "w");
    if (fp == NULL) {
        fputs("Error creating joke file\n", csp);
        return 1;
    }
    
    if (fprintf(fp, "%s", edited_joke) < 0) {
        fputs("Error editing joke file\n", csp);
        fclose(fp);
        return 1;
    }
    
    // Close the file
    if (fclose(fp) != 0) {
        fputs("Error closing joke file\n", csp);
        return 1;
    }
    
    fputs("Joke edited!", csp);
    return 0;
}
