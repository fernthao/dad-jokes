/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: delete.c
 Date created: Dec 1, 2025
 Description: Delete the joke with a given title under /jokes
*/
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

int delete_joke(char* title, FILE* csp) {
    if (title == NULL) {
        fputs("Can not delete with null title\n", csp);
        return 1;
    }
    // Construct the joke file path to be deleted
    char filepath[FILE_PATH_LEN];
    snprintf(filepath, FILE_PATH_LEN, "%s%s", JOKES_DIR, title);

    // Attempt to delete the file
    if(remove(filepath) == 0) {
        fputs("Joke deleted!\n", csp);
        return 0;
    } else {
        fputs("Error deleting joke\n", csp);
        return 1;
    }
}