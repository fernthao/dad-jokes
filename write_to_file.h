/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: write_to_file.h
 Date created: Dec 4, 2025
 Description: Write to the joke file
*/

#include <stdio.h>

#ifndef WRITE_JOKE_H
#define WRITE_JOKE_H

int write_to_file(char* joke_title, char* joke_content, FILE* csp, char* filepath);

#endif