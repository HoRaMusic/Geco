#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

#include <QVector>
#include <string>
#include <QStringList>
class blockmodel
{
public:
    blockmodel();
    void setNameVector(QStringList itemList);
    void setDefaultValuesVector(QStringList itemList);
    void setTypeVector(QStringList itemList);
    QVector<QString> getNameVector();
    QVector<int> getTypeVector();
    QVector<QString> getDefaultValueVector();

private:
    QVector<QString> nameVector;
    QVector<int> typeVector;
    QVector<QString> defaultValueVector;
};

#endif // BLOCKMODEL_H
