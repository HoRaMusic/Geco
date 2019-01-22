
#include <math.h>

class SampleAndHold {
private:
	float input;
	float gate;
	float output;
public:
    SampleAndHold() :
		input(0),
		gate(0),
		output(0)
		{

		};
        void setinput(float _input);
        void setgate(float _gate);
        float getoutput();
};

