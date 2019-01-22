
#include <math.h>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <deque>
#include <vector>

class clockedCycle {
private:
        int length;
        float rate;
        float count;
        float reset;
        float trig;
        bool isPlaying;
        bool resetOn;
	float output;
        std::vector<float> buf;
public:
    clockedCycle() :
                length(0),
                rate(0),
                count(0),
                reset(0),
                trig(0),
                isPlaying(false),
                resetOn(false),
                output(0)
		{

		};
        void setlength(int _length);
        void setrate(float _rate);
        void setreset(float _reset);
        void settrig(float _trig);
        float getoutput();
};

