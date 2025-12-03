
/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: dadsay.c
 Date created: Dec 2, 2025
 Description: Print ASCII picture of a smiley face with a given message
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "constants.h"
#include "random_joke.h"

#define MARGIN_SMALL 2
#define MARGIN_LARGE 5

/* fill lines array with each line in a random joke, 
    calculate max line length,
    and return the number of lines read 
*/
int dadsay_random_joke(char** lines, int* max_len, FILE* csp) {
    *max_len = 0;
    int line_count = 0;

    struct dirent* dir_entry;
    DIR* dir = opendir(JOKES_DIR);
    if (dir == NULL) {
        fputs("Error: Unable to open the jokes directory. Have you created any joke?\n", csp);
        return -1;
    }

    int random = random_joke_index();
    if (random < 0) {
        fputs("Error: Empty jokes directory. Add some jokes!\n", csp);
        return -1;
    }
    int index = 1;

    // Find the file at that index
    while ((dir_entry = readdir(dir)) != NULL) {
        // Skip current and parent dir
        if (strcmp(dir_entry->d_name,".") == 0 || 
            strcmp(dir_entry->d_name,"..") == 0
        ) {
            continue;
        }
        if (index == random) {
            // when found, open the file
            char* filename = dir_entry->d_name;
            char pathname[strlen(JOKES_DIR)+strlen(filename)+1];
            strcpy(pathname, JOKES_DIR);
            strcat(pathname, filename);
            FILE* joke = fopen(pathname, "r");
            if (joke == NULL) {
                fputs("Unable to open file\n", csp);
                closedir(dir);
                return -1;
            }
            // Get each line and add to the lines array
            char buffer[JOKE_LINE_LENGTH];
            while (fgets(buffer, JOKE_LINE_LENGTH, joke) != NULL && line_count < MAX_LINES) {
                // Remove newline character if present
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len-1] == '\n') {
                    buffer[len-1] = '\0';
                    len--;
                }
                
                // Allocate memory and copy the line to array of lines
                lines[line_count] = malloc(len + 1);
                if (lines[line_count] == NULL) {
                    fputs("Memory allocation failed\n", csp);
                    fclose(joke);
                    closedir(dir);
                    return -1;
                }
                strcpy(lines[line_count], buffer);
                
                // Compute max line length
                if ((int)len > *max_len) *max_len = (int)len;
                line_count++;
            }
            fclose(joke);
            break;
        }
        index++;
    }

    closedir(dir);
    return line_count;
}

// Writing to client socket pointer
int dadsay(FILE* csp) {
    char* lines[MAX_LINES];
    int max_len = 0;
    int line_count = dadsay_random_joke(lines, &max_len, csp);
    if (line_count == -1) {
        return 1;
    }
    
    // top bar
    fputs(" ", csp);
    for (int i = 0; i < max_len + MARGIN_SMALL; i++) fputs("_", csp);
    fputs("\n", csp);
    
    // lines with side borders
    for (int i = 0; i < line_count; i++) {
        char border = (line_count == 1) ? '<' : (i == 0) ? '/' : 
                      (i == line_count - 1) ? '\\' : 
                       '|';
        char line[max_len];
        snprintf(line, max_len + MARGIN_LARGE, "%c %-*s %c\n", border, max_len, lines[i], border);
        fputs(line, csp); 
        fputs("\n", csp);
    }
    
    // bottom bar
    fputs(" ", csp);
    for (int i = 0; i < max_len + 2; i++) fputs("-", csp);
    fputs("\n", csp);
    
    // line to speech bubble
    fputs("        \\\n         \\\n", csp);

    // dad
    fputs("              .-\"\"\"\"\"\"-.           \n", csp);
    fputs("            .'          '.         \n", csp);
    fputs("           /   O    -=-   \\        \n", csp);
    fputs("          :                :       \n", csp);
    fputs("          |                |       \n", csp);
    fputs("          : ',          ,' :       \n", csp);
    fputs("           \\  '-......-'  /        \n", csp);
    fputs("            '.          .'         \n", csp);
    fputs("              '-......-'           \n", csp);
    
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    return 0;
}