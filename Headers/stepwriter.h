#ifndef STEPWRITER_H
#define STEPWRITER_H
#include <QString>
#include "Headers/qneblock.h"
#include "Headers/qneport.h"
#include "Headers/qneconnection.h"
#include "Headers/customdialog.h"
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include  <QTextStream>
#include <QFileDialog>
class QGraphicsScene;
class QGraphicsItem;

class StepWriter: public QObject
{
    Q_OBJECT

public:
    explicit StepWriter(QObject *parent = 0);
    void fillBlockVector(QGraphicsScene *scene, QString _plugDir);
    void fillStepFunction();
    void writeInstuctions();
    QString inputInstruction(QNEBlock *currentBlock);
    QString outputInstruction(QNEBlock *currentBlock);
    QString genericInstruction(QNEBlock *currentBlock);
    QString extClassInstruction(QNEBlock *currentBlock);
    QString controllerInstruction(QNEBlock *currentBlock);
    QString ledInstruction(QNEBlock *currentBlock);
    QString operationInstruction(QNEBlock *currentBlock);
    QString variableInstruction(QNEBlock *currentBlock);
    QString stringInstruction(QNEBlock *currentBlock);
    QString arrayInstruction(QNEBlock *currentBlock);
    QString screenInstruction(QNEBlock *currentBlock, QString screen_name);
    QString counterInstruction(QNEBlock *currentBlock);
    QString gateToTrigInstruction(QNEBlock *currentBlock);
    QString clockInstruction(QNEBlock *currentBlock);
    QString nMinus1Instruction(QNEBlock *currentBlock);
    QString envInstruction(QNEBlock *currentBlock);
    QString sampleRateInstruction(QNEBlock *currentBlock);
    QString toJsonIntInstruction(QNEBlock *currentBlock);
    QString toJsonBoolInstruction(QNEBlock *currentBlock);
    QString toJsonRealInstruction(QNEBlock *currentBlock);
    QString numConverterInstruction(QNEBlock *currentBlock, int type);

private:
    QGraphicsScene *m_scene;
    QVector<QNEBlock*> smart_block;
    QVector<QString>FileToWriteInList;
    int numberOfBlocks;
    int numberOfPrevSampleBlocks;
    int numberOfOperations;
    int numberOfVultEngines;
    QString plugin_File_name;
    QString module_name;
    QVector<QString> instructions;
    QVector<QString> json_bool;
    QVector<QString> json_int;
    QVector<QString> json_real;
    QVector<QString> structInstructions;
    QVector<QString> vultEngines;
    QVector<QString> extClasses;
    QVector<QString> displayNames;
    QVector<QString> displayStructNames;
    QVector<QString> waveTables;
    bool vultUsed;
    bool extClassUsed;
    bool jsonUsed;
    bool screenUsed = true;
    bool followerUsed;
    bool includeFollowerWrited;
    bool getSampleRateWrited;
    bool includeVultWrited;
    bool includeExtClassUsedWrited;
    bool samplerateUsed;
    bool digitalDSPUsed;
    bool includeDigitalDSPWrited;
    bool jsonWrited;
    bool screenStructWrited;
    bool stepWrited;
    QString textColorR;
    QString textColorG;
    QString textColorB;
    QString textSize;
    QString fontName;
    QString plugDir;
};

#endif // STEPWRITER_H
