/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: write_to_file.c
 Date created: Dec 4, 2025
 Description: Write to the joke file
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

int write_to_file(char* joke_title, char* joke_content, FILE* csp, char* filepath) {
    // Count the number of lines in the joke
    int line_count = 0;
    int current_line_length = 0;
    for (int i = 0; joke_content[i] != '\0'; i++) {
        if (joke_content[i] == '\n') {
            line_count++;
            current_line_length = 0;
        } else {
            current_line_length++;
            // Enforce JOKE_LINE_LENGTH constraint
            if (current_line_length > JOKE_LINE_LENGTH) {
                char err[ERR_MSG_LEN];
                snprintf(err, ERR_MSG_LEN, "Error: Line %d exceeds maximum length of %d characters\n", line_count + 1, JOKE_LINE_LENGTH);
                fputs(err, csp);
                return ERROR;
            }
        }
    }
    // Count the last line if it doesn't end with newline
    if (strlen(joke_content) > 0 && joke_content[strlen(joke_content) - 1] != '\n') {
        line_count++;
    }
    
    // Enforce MAX_LINES constraint
    if (line_count > MAX_LINES) {
        char err[ERR_MSG_LEN];
        snprintf(err, ERR_MSG_LEN, "Error: Joke exceeds maximum of %d lines (has %d lines)\n", MAX_LINES, line_count);
        fputs(err, csp);
        return ERROR;
    }

    // Open file for writing, create if not existed
    FILE* fp = fopen(filepath, "w");
    if (fp == NULL) {
        fputs("Error opening joke file\n", csp);
        return ERROR;
    }
    
    if (fprintf(fp, "%s", joke_content) < 0) {
        fputs("Error writing to joke file\n", csp);
        fclose(fp);
        return ERROR;
    }
    
    // Close the file
    if (fclose(fp) != 0) {
        fputs("Error closing joke file\n", csp);
        return ERROR;
    }
    
    return 0;
}