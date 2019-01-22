#ifndef PROPERTY_H
#define PROPERTY_H
#include <QString>

class Property
{
public:
    Property(QString name, int type, float minValue, float maxValue);
    QString getName();
    int getType();
    float getMinValue();
    float getMaxValue();

private:
    QString name;
    int type;
    float minValue;
    float maxValue;
};

#endif // PROPERTY_H
