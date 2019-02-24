#include "clockedCycle.h"

void clockedCycle::setlength(int _length)
{
    length = _length;
}
void clockedCycle::setrate(float _rate)
{
     rate = _rate;
}
void clockedCycle::setreset(float _reset)
{
    if (_reset > 0)
    {
        resetOn = true;
        count = 0;
        isPlaying = false;
    }
    else
    {
        resetOn = false;
    }
}
void clockedCycle::settrig(float _trig)
{
        trig = _trig;
}
float clockedCycle::getoutput()
{
    if (trig != 0 && resetOn == false)
    {
        count = 0;
        isPlaying = true;
    }
    if (isPlaying == true)
    {
         count = count + rate;
    }
    if (count >= length || count < 0)
    {
        count = 0;
        isPlaying = false;
    }
    output = count;
    return output;
}
