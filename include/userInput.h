#ifndef USERINPUT_H
#define USERINPUT_H

#include "passwdGen.h" //Required for bool type

//Parses user input one string at a time
void parse_arguments(int argc, char *argv[]);

void sanitizeString(char *);

//Generic error message
void error_message();

//Help message
void help_message();

//Warning message
void warn_insecure(char *);

//Sets global length variable using user input
void set_length(char *);

//Sets global include variables based on user input
void set_include(char *);

//Sets word to base the password around
void set_word(char *);

//Parses user input string to check if it is an option
bool parse_option(char *);

#endif
