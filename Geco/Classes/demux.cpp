#include "demux.h"

void demux::setinput(float _input)
{
    input = _input;
}
void demux::setindex(float _index)
{
     index = _index;
}
float demux::getoutput1()
{
    float output = 0;
    if (index == 0)
    {
        output = input;
    }
    return output;
}
float demux::getoutput2()
{
    float output = 0;
    if (index == 1)
    {
        output = input;
    }
    return output;
}
float demux::getoutput3()
{
    float output = 0;
    if (index == 2)
    {
        output = input;
    }
    return output;
}
float demux::getoutput4()
{
    float output = 0;
    if (index == 3)
    {
        output = input;
    }
    return output;
}
float demux::getoutput5()
{
    float output = 0;
    if (index == 4)
    {
        output = input;
    }
    return output;
}
float demux::getoutput6()
{
    float output = 0;
    if (index == 5)
    {
        output = input;
    }
    return output;
}
float demux::getoutput7()
{
    float output = 0;
    if (index == 6)
    {
        output = input;
    }
    return output;
}
float demux::getoutput8()
{
    float output = 0;
    if (index == 7)
    {
        output = input;
    }
    return output;
}
