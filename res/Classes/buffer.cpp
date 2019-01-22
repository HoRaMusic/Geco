#include "buffer.h"

void buffer::setinput(float _input)
{
    input = _input;
}
void buffer::setrec(float _rec)
{
     rec = _rec;
}
void buffer::setreset(float _reset)
{
    if (_reset != 0)
    {
        index = 0;
    }
}
void buffer::setread(float _read)
{
     read = _read;
}
float buffer::getoutput()
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
        if(index < buf.size() - 1)
        {
            index ++;
        }
        else
        {
            index = 0;
        }
        if (buf.size()>0)
        {
            output = buf.at(index);
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
    return output;
}
