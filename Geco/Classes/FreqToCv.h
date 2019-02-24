
#include <math.h>

class FreqToCv {
private:
	float input;
	float base;
	float output;
public:
    FreqToCv() :
		input(0),
		base(0),
		output(0)
		{

		};
        void setinput(float _input);
        void setbase(float _base);
        float getoutput();
};

