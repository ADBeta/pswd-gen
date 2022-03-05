#include "generate.h"
#include <stdlib.h> //required for srand and free
#include <stdio.h> //required for size_t
#include <string.h> //required for stlen etc
#include <time.h> //reqired for seed function

#include <ctype.h> //Not super portable. if breaks, all that is needed is the
//isupper, islower, isnumber and issymbol functions

#include "passwdGen.h" //required for globals
#include "userInput.h" //Required for error messages

//Generic function to produce a random output, containing specified chars, of
//specified length.
char* gen_rand_str(unsigned int str_len, char include[5]) {
	//Allocate a char array of input + 1 for \0
	char *str = malloc(str_len + 1);
	
	unsigned int oc = 0; //output char
	while(oc < str_len) {
		/* ASCII values between 33 and 126 are all the usable chars.
		126 - 33 = 93 (+1 for modulo) */
	 	char randChar = (rand() % 94) + 33;
	 	bool allowedChar = false;
	 	
	 	//If upper is enabled, check current char to see if it matches
	 	if(include[0] == 'u' && isupper(randChar)) {
 			allowedChar = true;
	 	}
	 	
	 	//Same as above, but for lower, number and symbol
	 	if(include[1] == 'l' && islower(randChar)) {
 			allowedChar = true;
	 	}
	 	
	 	if(include[2] == 'n' && isdigit(randChar)) {
 			allowedChar = true;
	 	}
	 	
	 	if(include[3] == 's' && ispunct(randChar)) {
 			allowedChar = true;
	 	}
	 	
	 	//If the char being checked matched a criteria:
	 	if(allowedChar == true) {
	 		//Set current position in string to that char and move on
 			str[oc] = randChar;
 			++oc;
	 	}
	 	
	 	//If char didn't match any of the enables categories, it will be ignored and
	 	//the loop will contine with the current oc value and try again.	 	
	}
	//Set last char of output to \0 to stop over-runs
	str[oc] = '\0';
	
	//Return pointer location
    return str;
}

/* Generation functions */
void seed_random() {
	//Cast time from time_t to largest value availible.
	//Depending on time_t system type, could break at year 2038
	//size_t -should- make this code work forever if time.h is kept updated
	srand((size_t)time(NULL));
}

void gen_pwd() {
	//Fill in any varibles the user didn't provide 
	set_defaults();	
	
	//printf("Length: %lu \t Include: %s\n", g_settings.length, g_settings.include);
	//printf("Word length: %lu \t Word: %s\n", g_settings.wordLength, g_settings.word);
	
	//Do last sanity checks
	//If user provided word is larger or equal the requested output, error
	if(g_settings.wordLength >= g_settings.length) {
		printf("Word is longer than the length of the password.\n");
		error_message();
	}
	
	//Allocate heap memory for output string
	g_output = malloc(g_settings.length + 1);
	
	//Basic password
	if(g_settings.wordSet == false) {
		//Generate the random password and move it into the output string
		g_output = gen_rand_str(g_settings.length, g_settings.include);
	}
	
	if(g_settings.wordSet == true) {
		//Generate random string, required length - word length, move it to output
		unsigned int outputLength = g_settings.length - g_settings.wordLength;
		g_output = gen_rand_str(outputLength, g_settings.include);
		
		//Get random starting position between 0 and last char of output
		unsigned int startPtr = rand() % outputLength + 1;
		
		//Char for keeping track of char that it is running over
		char overrunChar = '\0';
		
		//Copy word string into output from startChar
		//Move any char that is being over-run to the end of the string
		for(unsigned int wordPtr = 0; wordPtr < g_settings.wordLength; wordPtr++) {
			//Current char in g_output
			unsigned int outputPtr = startPtr + wordPtr;
			
			//If current char to be replaced is not empty then get a copy of it
			if(g_output[outputPtr] != '\0') {
				overrunChar = g_output[outputPtr];
			}
			
			//Copy char from word into position in output
			g_output[outputPtr] = g_settings.word[wordPtr];
			
			//Put overrun char at the end of the output if needed
			if(overrunChar != '\0') {
				g_output[outputLength + wordPtr] = overrunChar;
			}
		}
		
		//free ram used by the word string
		free(g_settings.word);
		g_settings.word = NULL;
	}
	
	//Output string
	printf("\nPassword:\t%s\n", g_output);
	
	
	
	//Zero out the g_output string for safety
	for(unsigned int outputChar = 0; outputChar < g_settings.length+1; outputChar++) {
		g_output[outputChar] = 0;
	}
	//Free the output string
	free(g_output);
	g_output = NULL;
}
