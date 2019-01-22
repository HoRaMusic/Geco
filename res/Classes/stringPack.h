
#include <math.h>
#include <string.h>
#include <stdio.h>

class stringPack {
private:
	char input1[100];
	char input2[100];
	char input3[100];
	char input4[100];
	char input5[100];
	char output[100];

public:
    stringPack() :
		input1(""),
		input2(""),
		input3(""),
		input4(""),
		input5(""),
		output(""),
		{

		};
        void setstr1(char *_input);
        void setstr2(char *_input);
        void setstr3(char *_input);
        void setstr4(char *_input);
        void setstr5(char *_input);
        char * getoutput();
};

