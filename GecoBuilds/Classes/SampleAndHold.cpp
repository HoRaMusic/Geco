#include "SampleAndHold.h"

void SampleAndHold::setinput(float _input)
{
    input = _input;
}
void SampleAndHold::setgate(float _gate)
{
     gate = _gate;
}
float SampleAndHold::getoutput()
{
    if (gate > 0.5)
    {
        output = input;
    }
    return output;
}
