/*
	Generates a random password with multiple modes.


	Written by ADBeta
	Modified 19 Oct 2021
	
	Version 0.66
*/

/* avoiding using too many libraries. Defined bool manually */
#include <stdio.h>
#include <stdlib.h> //required for free
#include <string.h>

/* external code headers */
#include "passwdGen.h" //global variabes and struct
#include "userInput.h" //User input handling
#include "generate.h" //Generation code

//Global settings struct and define some vars
struct Settings g_settings = {
};
//Global output string
char *g_output;

void set_defaults() {
	if(g_settings.lengthSet == false) {
		g_settings.length = 16;
		g_settings.lengthSet = true;
	}
	
	if(g_settings.includeSet == false) {
		strcpy(g_settings.include, "ulns");
		g_settings.includeSet = true;
	}
}

int main(int argc, char *argv[]) {	
	//Pass arguments to function, excluding exec path
	parse_arguments(argc-1, argv+1);
	
	//Seed the RNG values
	seed_random();
	
	//Generate random password
	gen_pwd();
	
	return 0;
}
