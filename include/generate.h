#include <stdbool.h> //Bool type
#include <stdio.h> //size_t

#ifndef GENERATE_H
#define GENERATE_H

/* global variables */
struct Settings {
	//Length of requested password
	size_t length;
	
	//Char types to include in the output password (u l n s)
	char include[5];
	
	//Input word to base the password gen on, and size of that string
	char *word;
	unsigned int wordLength;
	
	//Keep track of if variables have been set (may be unneeded)
	bool lengthSet, includeSet, wordSet;
};

void set_defaults();

extern struct Settings g_settings;

//Output string to use at the end of the program, malloc'ed then free'd in main
extern char *g_output;




char* gen_rand_str(unsigned int, char[5]);

void seed_random();

void gen_pwd();

#endif
