/* pswd-gen is a highly efficient and secure password generator, ideal for 
*  using with scripts to generate new, custom passwords whenever necessary.
*
*  Some features include:
*
*  This software is under the GPL licence. Please see LICENCE for more info.
*  v2.0.2
*  ADBeta Dec 2022
*/

#include "generate.h"
#include "userInput.h"

int main(int argc, char *argv[]) {	
	//Pass arguments to function, excluding exec path
	parse_arguments(argc-1, argv+1);
	
	//Seed the RNG values
	seed_random();
	
	//Generate random password
	gen_pwd();
	
	return 0;
}

