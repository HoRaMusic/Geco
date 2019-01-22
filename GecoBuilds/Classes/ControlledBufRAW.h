
#include <math.h>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <deque>
#include <vector>

class ControlledBufRAW {
private:
	float input;
        float rec;
        bool wasOnRec;
        float read;
        int index;
        float reset;
	float output;
        std::vector<float> buf;
public:
    ControlledBufRAW() :
		input(0),
                rec(0),
                wasOnRec(false),
                read(0),
                index(0),
                reset(0),
                output(0),
                buf()
		{

		};
        void setinput(float _input);
        void setrec(float _rec);
        void setread(float _read);
        void setreset(float _reset);
        void setindex(int _index);
        float getoutput();
        int getbufSize();
};

