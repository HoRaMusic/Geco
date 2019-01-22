#include "blockmodel.h"

blockmodel::blockmodel()
{

}
void blockmodel::setTypeVector(QStringList itemList)
{
    for (int i = 0; i < itemList.size(); ++i)
    {
        if (i%3 == 0)
        {
            QString d = itemList.at(i);
            typeVector.append(d.toInt());
        }
    }
}

void blockmodel::setNameVector(QStringList itemList)
{
    for (int i = 0; i < itemList.size(); ++i)
    {
        if (i%3 == 1)
        {
            QString d = itemList.at(i);
            nameVector.append(d);
        }
    }
}

void blockmodel::setDefaultValuesVector(QStringList itemList)
{
    for (int i = 0; i < itemList.size(); ++i)
    {
        if (i%3 == 2 && i >= 6)
        {
            QString d = itemList.at(i);
            defaultValueVector.append(d);
        }
    }
}

QVector<QString> blockmodel::getNameVector()
{
    return nameVector;
}
QVector<int> blockmodel::getTypeVector()
{
    return typeVector;
}
QVector<QString> blockmodel::getDefaultValueVector()
{
    return defaultValueVector;
}
