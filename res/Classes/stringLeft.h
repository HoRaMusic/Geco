
#include <math.h>
#include <string.h>
#include <stdio.h>

class stringLeft {
private:
	char input[100];
	int numberOfCharacters;
	char output[100];

public:
    stringLeft() :
		input(""),
		numberOfCharacters(0),
		output("")
		{

		};
        void setstring(char *_input);
        void setlength(int _numberOfCharacters);
        char * getoutput();
};

