#include "ControlledBufRAW.h"

void ControlledBufRAW::setinput(float _input)
{
    input = _input;
}
void ControlledBufRAW::setrec(float _rec)
{
     rec = _rec;
}
void ControlledBufRAW::setindex(int _index)
{
        index = _index;
    if (index >= buf.size())
    {
        index = 0;
    }
}
void ControlledBufRAW::setreset(float _reset)
{
    if (_reset != 0)
    {
        index = 0;
    }
}
void ControlledBufRAW::setread(float _read)
{
     read = _read;
}
float ControlledBufRAW::getoutput()
{
    if (rec != 0 && wasOnRec == false)
    {
        buf.clear();
        index = 0;
        wasOnRec = true;
        buf.push_back(input);
        output = 0;
    }
    else if (rec != 0)
    {
        buf.push_back(input);
        output = 0;
    }
    else if (read != 0)
    {

        if (buf.size()>0 && index < buf.size())
        {

            float lValue =  buf.at(index);
            output = lValue;
        }
        else
        {
            output = 0;
        }
    }
    if (rec == 0)
    {
        wasOnRec = false;
    }
    if (read == 0)
    {
    output = 0;
    }
    return output;
}
int ControlledBufRAW::getbufSize()
{
    return buf.size();
}
