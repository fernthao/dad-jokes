/* Name: Thao Nguyen 
 Case Network ID: ttn60
 The filename: dadsay.h
 Date created: Dec 2, 2025
 Description: Print ASCII picture of a smiley face with a given message
*/
#include <stdio.h>

#ifndef DAD_SAY_H
#define DAD_SAY_H

int dadsay_random_joke(char** lines, int* max_len);
int dadsay(FILE* csp);

#endif