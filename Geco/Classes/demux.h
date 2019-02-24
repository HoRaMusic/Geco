
#include <math.h>

class demux {
private:
        float input;
        float output1;
        float output2;
        float output3;
        float output4;
        float output5;
        float output6;
        float output7;
        float output8;

        float index;
public:
    demux() :
                input(0),
                output1(0),
                output2(0),
                output3(0),
                output4(0),
                output5(0),
                output6(0),
                output7(0),
                output8(0),
                index(0)
		{

		};
        void setinput(float _input);
        void setindex(float _index);
        float getoutput1();
        float getoutput2();
        float getoutput3();
        float getoutput4();
        float getoutput5();
        float getoutput6();
        float getoutput7();
        float getoutput8();
};

