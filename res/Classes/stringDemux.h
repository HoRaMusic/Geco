
#include <math.h>
#include <string.h>
#include <stdio.h>
class stringDemux {
private:
        char input[100];
        char output1[100];
        char output2[100];
        char output3[100];
        char output4[100];
        char output5[100];
        char output6[100];
        char output7[100];
        char output8[100];

        int index;
public:
    stringDemux() :
                input(""),
                output1(""),
                output2(""),
                output3(""),
                output4(""),
                output5(""),
                output6(""),
                output7(""),
                output8(""),
                index(0)
		{

		};
        void setinput(char * _input);
        void setindex(int _index);
        char * getoutput1();
        char * getoutput2();
        char * getoutput3();
        char * getoutput4();
        char * getoutput5();
        char * getoutput6();
        char * getoutput7();
        char * getoutput8();
};

