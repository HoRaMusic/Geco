#include "Headers/property.h"

Property::Property(QString _name, int _type, float _minValue, float _maxValue)
{
    name = _name;
    type = _type;
    minValue = _minValue;
    maxValue = _maxValue;
}


QString Property::getName()
{
    return name;
}
int Property::getType()
{
    return type;
}
float Property::getMinValue()
{
    return minValue;
}
float Property::getMaxValue()
{
    return maxValue;
}
