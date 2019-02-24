#include "FreqToCv.h"

void FreqToCv::setinput(float _input)
{
    input = _input;
}
void FreqToCv::setbase(float _base)
{
     base = _base;
}
float FreqToCv::getoutput()
{
   float output = log2f(input / base);;
   return output;
}
