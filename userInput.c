#include "userInput.h" 
#include "passwdGen.h" //Required for globals
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Generic error message for how to use this software
void error_message() {
	printf("Usage: pwdgen [OPTION]...\n"
	"Use psswdgen --help for more information\n");
	
	exit(EXIT_FAILURE);
}

//Specific message given when user requests help (--help or -h)
void help_message() {
	printf("Usage: pwdgen [OPTION]....\n"
	"Generates a random password according to the options.\n\n"
	
	"  length=LEN\t\tset the length of the output (default: 16 | max: 512)\n"
	
	"  include=CHARS\t\tspecify which character types to include (default: ulns)\n"
	"  \t\t\tu=uppercase, l=lowecase, n=numeric, s=symbol.\n"
	
	"  word=WORD\t\tspecify a word that you want the password to include at some point in the output.\n"
	
	"\n\nExamples:\n"
	"pwdgen\t\tZ>Q7[m`Fo54<z|om\n"
	"Generates a 16 length password using all availible characters\n\n"
	
	"pwdgen include=ul length=16\t\tKcoUmjeHLYbEcOxG\n"
	"Generates a 16 long password including only Uppercase and Lowercase characters\n\n"
	
	"pwdgen word=example length=16\t\t$,I@LQexample._;\n"
	"Generates a 16 character long password including the word \"example\", using all availible characters\n"
	);
	
		exit(EXIT_SUCCESS);
}

void warn_insecure(char *warningMsg) {
	printf("Warning: %s, doing this may cause\n"
	"this password to be weak or insecure. Please excesise caution using it.\n"
	, warningMsg);
}

//Sets global length variable using user input
void set_length(char *operand) {
	//Exit if string given is larger than expected
	if(strlen(operand) > 3) {
		printf("Length string too long.\n");
		error_message();
	}
	
	//convert string to int in base 10
	size_t strNum = strtol(operand, NULL, 10);
	
	//error checking
	if(strNum == 0) {
		printf("Length input not recognized.\n");
		error_message();
	}
	
	//Warning for short password
	if(strNum < 10) {
		warn_insecure("Password is under 10 characters long");
	}
	
	//Exit if size is too large
	if(strNum > 512) {
		printf("Length exceeds maximum.\n");
		error_message();
	}
	
	g_settings.length = strNum;
	g_settings.lengthSet = true;
}

//Sets global include variables based on user input
void set_include(char *operand) {
	//Exit if string given is larger than expected
	if(strlen(operand) > 4) {
		printf("Include string too long\n");
		error_message();
	}
	
	//Set the include string to all disabled before changing
	strcpy(g_settings.include, "----");
	
	int cc = 0; //Current char
	while(operand[cc] != '\0') {
	
		if(operand[cc] == 'u') {
			g_settings.include[0] = 'u';
		}
			
		else if(operand[cc] == 'l') {
			g_settings.include[1] = 'l';
		}
		
		else if(operand[cc] == 'n') {
			g_settings.include[2] = 'n';
		}
		
		else if(operand[cc] == 's') {
			g_settings.include[3] = 's';
		}		

		else {
			//If char doesn't match any option, error
			printf("Include option not recognised.\n");
			error_message();
		}
		++cc;
	}
	
	g_settings.includeSet = true;
}

void set_word(char *operand) {
	g_settings.wordLength = strlen(operand);

	//Exit if string given is larger than expected
	if(g_settings.wordLength > 20) {
		printf("Input word exceeds maximum.\n");
		error_message();
	}
	
	warn_insecure("Using a plain-text word in your password");
	
	//If word is 0 length then exit without complaining
	if(g_settings.wordLength == 0) return;
	
	//Malloc space and copy string into mem
	g_settings.word = malloc(g_settings.wordLength + 1);
	strcpy(g_settings.word, operand);
	
	g_settings.wordSet = true;	
}

//Parses user input string to check if it is an option
bool parse_option(char *input) {
	bool isOption = false;
	char delim = '=';
	
	/* Make list of keywords, and derive list size from that. Allows 
	upgrades later on */
	const char *keywordList[] = {"include", "length", "word"};
	
	//Get size of element in chars
	int elementSize = sizeof(keywordList[0]);
	
	//Get number of elements in the list
	int listSize = sizeof(keywordList) / sizeof(keywordList[0]);
	
	/* Create dynamic string for use in searching. same size as an entry in 
	keywordList */
	char *keyword = malloc(elementSize);
	
	//Go through all keywords
	for(int kw = 0; kw < listSize; kw++) {
		strcpy(keyword,keywordList[kw]);
	
		//Scan through chars in input (from 0) while it matches the input
		int cc = 0; //Current Char
		//Start from char[0], until char in keyword is \0 and not matching.
		while(keyword[cc] == input[cc] && keyword[cc] != '\0') {
			++cc;
		}
		
		//At this point it has gone through all matching chars,
		//if the keyword char is \0 and input char is delim then the user entered
		//a known keyword. We can then parse the value given by keyword
		if(keyword[cc] == '\0' && input[cc] == delim) {
			//used for return status, to let later execution know the input
			//was regognised
			isOption = true;
				
			//Incriment cc past delim
			++cc;
			
			//Make sure there is actual information in the option string
			if(input[cc] == '\0') {
				printf("No argument given for %s.\n", keyword);
				error_message();
			}
			
			//Move remaining chars into a fresh array for parsing.
			//inputSize is + 1 to fit \0, then value is inputSize
			//minus the size of the pre-delim string
			int inputSize = strlen(input) + 1;
			char *operand = malloc(inputSize - cc);
			
			//Move post-delim chars to new string
			int oc = 0; //Operand Char
			while(cc < inputSize) {
				//vc starts at 0, cc starts at post-delim char
				operand[oc] = input[cc];
				++cc;
				++oc;
			}
			
			//Now we have a sepeate string with the value given by the argument
			//and know which argument was given due to keyword string and kw
			
			//Include
			if(strcmp(keyword, keywordList[0]) == 0) {
				set_include(operand);
			}
			
			//Length
			if(strcmp(keyword, keywordList[1]) == 0) {
				set_length(operand);
			}
			
			//Word
			if(strcmp(keyword, keywordList[2]) == 0) {
				set_word(operand);
			}
			
			//Free up the malloc'd post-delim string
			free(operand);
			operand = NULL;
		}
	}	
	//Finish
	free(keyword);
	keyword = NULL;
	//Return true or false if the current string is an option
	return isOption;
}

//Parses user input string to check if it is an argument
void parse_arguments(int argc, char *argv[]) {
/*	Possible failures:
	no arguments given.
	mismatching arguments:
	unknown command
*/	
	//If no arguments given, escape without doing checks
	if(argc == 0) {
		printf("Using default options.\n");
		return;
	}
	
	int argp = 0; //Argument pointer
	/* run through all arguments */
	while(argp < argc) {
		//Escape if input string is over 64 chars long
		if(strlen(argv[argp]) > 64) {
			printf("Argument too long.\n");
			error_message();
		}
		
		//completely override everything else if help is requested
		if(strcmp(argv[argp], "--help") == 0)
			help_message();
		
		//Parse for options 
		bool isOption = parse_option(argv[argp]);
		
		//If current input isn't recognised, exit
		if(isOption == false) {
			printf("Unrecognised input: %s.\n", argv[argp]);	
			error_message();
		}
		
		//Continue through loop
		++argp;
	}
}
