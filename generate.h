#ifndef GENERATE_H
#define GENERATE_H

#include <stdio.h> //required for size_t

#include "passwdGen.h" //bool type

char* gen_rand_str(unsigned int, char[5]);

void seed_random();

void gen_pwd();

#endif
