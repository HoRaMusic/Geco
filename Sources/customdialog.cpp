#include "customdialog.h"

//############################################################

#include <qstringlist.h>
#include <qvalidator.h>
#include <qcolordialog.h>

using namespace std;

//############################################################

ColorButton::ColorButton(QColor _color, QWidget *parent) : QPushButton(parent)
{
  color = _color;
  this->setStyleSheet("background-color: " + color.name());
  this->setMaximumSize(20,20);
  QObject::connect(this, SIGNAL(clicked()), this, SLOT(pickColor()));  // Won't work in Qt 4.3.
}

void ColorButton::setColor(QColor _color)
{
  color = _color;
  this->setStyleSheet("background-color: " + color.name());
}

QColor ColorButton::getColor()
{
  return (color);
}

void ColorButton::pickColor()
{
  QColor newColor = QColorDialog::getColor(color, this);
  if(newColor.isValid())
  {
    color = newColor;
    //color = QColorDialog::getColor(color, this);
    //QColorDialog dlgColor = new QColorDialor();
    //if(dlgColor.exec())
    //color = dlgColor.selectedColor();
    this->setStyleSheet("background-color: " + color.name());
      // NOTE: QColorDialog is significantly different in Qt version 4.3 versus 4.7
    emit released();
  }
}


//############################################################

//------------------------
//-- Default construtor

CustomDialog::CustomDialog(QString title, QWidget *parent, btnset btnSet)
  : QDialog(parent)
{
  setWindowTitle(title);


  QVBoxLayout *vboxLayoutMain = new QVBoxLayout(this);
  vboxLayoutMain->setSpacing(0);
  vboxLayoutMain->setMargin(0);

  vboxLayout = new QVBoxLayout();
  vboxLayout->setSpacing(5);
  vboxLayout->setMargin(8);
  vboxLayout->setContentsMargins(9, 6, 9, 9);
  layoutNextElement = vboxLayout;

  //## ADD "CANCEL" AND "OKAY" BUTTONS AT BOTTOM:

  hbtnLayout = new QHBoxLayout();
  hbtnLayout->setSpacing(5);
  hbtnLayout->setMargin(8);


  if(btnSet == BS_CANCEL_OKAY)
  {
    addCustomButton("Cancel", BB_REJECT);
    addCustomButton("Okay",   BB_ACCEPT);
  }
  else if(btnSet == BS_OKAY_ONLY)
  {
    addCustomButton("Okay", BB_ACCEPT);
  }
  else if(btnSet == BS_NO_YES)
  {
    addCustomButton("No",  BB_REJECT);
    addCustomButton("Yes", BB_ACCEPT);
  }

  vboxLayoutMain->addLayout(vboxLayout);
  vboxLayoutMain->addLayout(hbtnLayout);

  this->setLayout(vboxLayoutMain);
}



//------------------------
//-- Allows you to add a custom buttons to the bottom of the dialog.
//-- @ buttonStr   = the label on the button
//-- @ buttonBehav = the "behavior" of the button where:
//--                 "BB_ACCEPT" will update all values  (and dialog closes)
//--                 "BB_REJECT" won't update any values (and dialog closes)
//--                 "BB_DISABLED" means the button is disables (so does nothing)
//--                 "BB_POPUP" will display the tooltip in a message box (and not close)
//--                 "BB_OPENURL" will open a browser to the address specified
//--                 in the tooltip (and not close)
//-- @ tooltip  = optional tooltip

bool CustomDialog::addCustomButton(QString buttonStr, btnbehav buttonBehav,
                                    QString tooltip)
{
  customBtn.push_back(new QPushButton(buttonStr, this));

  QPushButton *newButton = customBtn.back();

  switch (buttonBehav)
  {
    case (BB_ACCEPT):
      QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(customBtnAccept()));
      break;
    case (BB_REJECT):
      QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(customBtnReject()));
      break;
    case (BB_DISABLED):
      newButton->setEnabled(false);
      break;
    case (BB_POPUP):
      QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(customBtnMessage()));
      break;
    case (BB_OPENURL):
      QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(customBtnOpenUrl()));
      break;
  }

  if(!tooltip.isEmpty())
    newButton->setToolTip(tooltip);

  hbtnLayout->addWidget(newButton);



  return true;
}


//------------------------
//-- This function is called at the start of most "DialogElement" functions to
//-- create a new "DialogElement" and add it to the end of the vector.
//-- In addition to creating the element, it also sets up "layout",
//-- and "makeLabel" is true it will initialize "label", but any other
//-- form elements must be added seperately.
//-- @ _type   = the type of dialog element to be created (see: DlgType)
//-- @ caption = the caption for the element, which (depending on its type) will be
//--             the text within the element or a label placed on the left of the element
//-- @ tooltip  = optional tooltip
//-- @ makeLabel = if true: a label will be added to the left of element.

DialogElement& CustomDialog::addNewElement(DlgType _type, QString caption,
                                            QString tooltip, bool makeLabel)
{
  DialogElement e;
  e.type = _type;
  e.extraChkAdded = false;

  if(makeLabel)
  {
    e.label = new QLabel(caption, this);
    if(!tooltip.isEmpty())
      e.label->setToolTip(tooltip);
    //setDefaultColorAndFont(e.label);  // Uncomment if you add special font or stylesheet changes to group boxes.
  }

  e.layout = new QHBoxLayout();
  e.layout->setSpacing(0);
  e.layout->setContentsMargins(0, 0, 0, 0);

  elements.push_back(e);

  return (elements.back());
}

//------------------------
//-- Adds a plain text label to the next row of the dialog
//-- @ caption  = the caption for the label
//-- @ bold     = set to true if you want the text to be bold
//-- @ tooltip  = optional tooltip

int CustomDialog::addLabel(QString caption, bool bold, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_LABEL, caption, tooltip, true);

  setBold(e.label, bold);
  e.label->setTextFormat(Qt::PlainText);
  e.layout->addWidget(e.label);
  //setTextColor(e.label,0,0,255);      // Makes labels blue.
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}


//------------------------
//-- Adds a html/rich text and hyperlink enabled label to the next row of the dialog
//-- @ caption  = the html/rich text caption for the label
//--              (eg: "<i>Made by</i>: <a href='www.andrewnoske.com'>Andrew<a>")
//-- @ tooltip  = optional tooltip

int CustomDialog::addHtmlLabel(QString caption, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_LABEL, caption, tooltip, true);

  e.label->setTextFormat(Qt::RichText);
  e.label->setOpenExternalLinks(true);
  e.layout->addWidget(e.label);
  //setTextColor(e.label,0,0,255);      // Makes labels blue.
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}


//------------------------
//-- Adds a checkbox to the next row of the dialog
//-- @ caption   = the caption for the checkbox (on the right of the box)
//-- @ *checked  = default value + where the checkbox value is updated if "Ok" is clicked
//-- @ tooltip   = optional tooltip

int CustomDialog::addCheckBox(QString caption, bool *checked, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_CHECKBOX, caption, tooltip, false);

  e.returnBool = checked;
  e.chkBox = new QCheckBox(caption, this);
  e.chkBox->setChecked(*checked);
  if(!tooltip.isEmpty())
    e.chkBox->setToolTip(tooltip);

  e.layout->addWidget(e.chkBox);
  layoutNextElement->addLayout(e.layout);

  return elements.size();
}

//------------------------
//-- Adds a line edit, preceeded by a label, to the next row of the dialog
//-- @ caption       = the caption for the label to the left of the line edit box
//-- @ *stringValue  = the default value for + where the line edit
//--                   value is saved if/when the user clicks "Ok"
//-- @ tooltip       = optional tooltip

int CustomDialog::addLineEdit(QString caption, string *stringValue, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_LINEEDIT, caption, tooltip, true);

  e.returnString = stringValue;
  e.lineEdit = new QLineEdit(this);
  e.lineEdit->setText(stringValue->c_str());
  if(!tooltip.isEmpty())
    e.lineEdit->setToolTip(tooltip);

  e.layout->addWidget(e.label);
  e.layout->addWidget(e.lineEdit);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a line edit, preceeded by a label, to the next row of the dialog
//-- @ caption       = the caption for the label to the left of the line edit box
//-- @ text          = the default value for + where the spin box
//--                   value is saved if/when the user clicks "Ok"
//-- @ tooltip       = optional tooltip

int CustomDialog::addReadOnlyLineEdit(QString caption, QString text, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_LABEL, caption, tooltip, true);

  e.lineEdit = new QLineEdit(this);
  e.lineEdit->setText(text);
  e.lineEdit->setReadOnly(true);
  if(!tooltip.isEmpty())
    e.lineEdit->setToolTip(tooltip);

  e.layout->addWidget(e.label);
  e.layout->addWidget(e.lineEdit);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a special line edit which accepts only floating poing numbers to the next row
//-- of the dialog
//-- @ caption   = the caption for the label to the left of the line edit box
//-- @ min, max  = minimum and maximum limits
//-- @ *value    = the default value for + where the value is saved if user clicks "Ok"
//-- @ decimal   = number of decimal points to show
//-- @ tooltip   = optional tooltip

int CustomDialog::addLineEditF(QString caption, float min, float max, float *value,
                                float decimals, QString tooltip, QString unitsStr)
{
  DialogElement &e = addNewElement(DLG_FLOATEDIT, caption, tooltip, true);

  e.returnFloat = value;
  e.lineEdit = new QLineEdit(this);
  e.lineEdit->setText(QString::number(*value));
  e.lineEdit->setMaximumWidth(100);
  if(!tooltip.isEmpty())
    e.lineEdit->setToolTip(tooltip);
  e.lineEdit->setValidator(new QDoubleValidator(min, max,
                           decimals, e.lineEdit));

  QSpacerItem *sp = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
  e.layout->addWidget(e.label);
  e.layout->addItem(sp);
  e.layout->addWidget(e.lineEdit);

  if(!unitsStr.isEmpty())
  {
    e.label2 = new QLabel(unitsStr,this);
    e.layout->addWidget(e.label2);
  }

  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a spin box, preceeded by a label, to the next row of the dialog
//-- @ caption   =  the caption for the label to the left of the spin box
//-- @ min, max  =  minimum and maximum (integer) limits for the spin box
//-- @ *value    =  default value + where the spin box value is saved if user clicks "Ok"
//-- @ step      =  the interval the spin box advances when its little arrows are clicked
//-- @ tooltip   =  optional tooltip


int CustomDialog::addSpinBox(QString caption, int min, int max, int *value, int step,
                              QString tooltip)
{
  DialogElement &e = addNewElement(DLG_SPINBOX, caption, tooltip, true);

  e.returnInt = value;
  e.spnBox = new QSpinBox(this);
  e.spnBox->setRange     (min, max);
  e.spnBox->setValue     (*value);
  e.spnBox->setSingleStep(step);
  if(!tooltip.isEmpty())
    e.spnBox->setToolTip(tooltip);

  QSpacerItem *sp = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
  e.layout->addWidget(e.label);
  e.layout->addItem(sp);
  e.layout->addWidget(e.spnBox);
  layoutNextElement->addLayout(e.layout);

  return elements.size();
}

//------------------------
//-- Adds a floating-point spin box, preceeded by a label, to the next row of the dialog
//-- @ caption  =  the caption for the label to the left of the spin box
//-- @ min, max =  minimum and maximum (floating point) limits for the spin box
//-- @ *value   =  the default value + the variable which gets updated if user clicks "Ok"
//-- @ decimal  =  number of decimal points to show/allow in the spin box
//-- @ step     =  the interval the spin box advances when its little arrows are clicked
//-- @ tooltip  =  optional tooltip

int CustomDialog::addDblSpinBoxF(QString caption, float min, float max, float *value,
                                  int decimals, float step, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_DBLSPINBOX, caption, tooltip, true);

  e.returnFloat = value;

  e.dblSpnBox = new QDoubleSpinBox(this);
  e.dblSpnBox->setRange(min, max);
  e.dblSpnBox->setValue     (*value);
  e.dblSpnBox->setSingleStep(step);
  e.dblSpnBox->setDecimals  (decimals);
  if(!tooltip.isEmpty())
    e.dblSpnBox->setToolTip(tooltip);

  QSpacerItem *sp = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
  e.layout->addWidget(e.label);
  e.layout->addItem(sp);
  e.layout->addWidget(e.dblSpnBox);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a combo box, preceeded by a label, to the next row of the dialog
//-- @ caption    =  the caption for the label to the left of the combo box
//-- @ barSepList =  sting of items for the combo list. Each item must be seperated by
//--                 a bar character (eg: "one|two").
//-- @ *selIdx    =  the default selected index + where the selected index is
//--                 updated when/if the user clicks "Ok"
//-- @ tooltip    =  optional tooltip

int CustomDialog::addComboBox(QString caption, QString barSepList, int *selIdx,
                               QString tooltip)
{
  DialogElement &e = addNewElement(DLG_COMBOBOX, caption, tooltip, true);

  e.returnInt = selIdx;
  e.cmbBox = new QComboBox(this);
  QStringList items = barSepList.split("|", QString::SkipEmptyParts);
    // break apart string values between each "|"

  e.cmbBox->addItems(items);
  if(*selIdx < (int)items.size())  // Set default selection.
    e.cmbBox->setCurrentIndex(*selIdx);
  if(!tooltip.isEmpty())
    e.cmbBox->setToolTip(tooltip);

  e.layout->addWidget(e.label);
  e.layout->addWidget(e.cmbBox);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a radio group to the next row of the dialog
//-- @ caption    = the caption/title of the group box around the radio buttons
//-- @ varSepList = sting of items/radio button for the radio box group - whereby
//--                each item must be seperated by bars (eg: "opt 1|opt 2|optn 3").
//-- @ *selIdx    = the default selected index + where the selected index is
//--                updated when/if the user clicks "Ok"
//-- @ tooltip    = optional tooltip applied to whole group
//-- @ tooltipArr = optional string of tool tips where each tool tip must be seperated by
//--                a bar character (eg: "tooltip opt1|tooltip opt2|tooltip opt 3")
//-- @ checkable  = wether or not a checkbox appears in the title of the radio group.
//-- @ *checked   = if "checkable" is true, the default value + where the value of the
//--                checkbox is updated when/if the user clicks "Ok"

int CustomDialog::addRadioGrp(QString caption, QString barSepList, int *selIdx,
                               QString tooltip, QString tooltipArr, bool checkable, bool *checked)
{
  DialogElement &e = addNewElement(DLG_RADIOGRP, caption, tooltip, false);

  e.returnInt = selIdx;
  e.grpBox = new QGroupBox(this);
  e.grpBox->setTitle(caption);
  if(checkable)
  {
    e.returnBool = checked;
    e.grpBox->setCheckable(*checked);
  }
  QButtonGroup *butGrp = new QButtonGroup(this);
  QVBoxLayout  *butLay = new QVBoxLayout(e.grpBox);
  butLay->setSpacing(2);
  butLay->setContentsMargins(5, 2, 5, 5);
  QStringList items = barSepList.split(
      "|",QString::SkipEmptyParts);   // Break apart string values between each "|".

  QStringList tooltips;
  if(!tooltipArr.isEmpty())
    tooltips = tooltipArr.split("|", QString::SkipEmptyParts);

  for (int j=0; j<(int)items.size(); j++)    // Add each of item as radio button.
  {
    QRadioButton *newRadBtn = new QRadioButton(e.grpBox);
    butLay->addWidget(newRadBtn);
    butGrp->addButton(newRadBtn, j);
    newRadBtn->setText(items[j]);
    if(j == *selIdx)
      newRadBtn->setChecked(true);        // Set default selection.
    if(j<(int)tooltips.size())
      newRadBtn->setToolTip(tooltips[j]);
    e.radBtn.push_back(newRadBtn);
  }

  if(!tooltip.isEmpty())
    e.grpBox->setToolTip(tooltip);

  e.layout->addWidget(e.grpBox);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a small square color selection button, preceeded by a label, to the dialog.
//-- When this button is clicked, Qt's color selector dialog appears, and the color of
//-- the button changes to the color selected
//-- @ caption    = the caption of the label to the left of the button
//-- @ *color     = the default color + where the color is updated if the user clicks "Ok"
//-- @ tooltip    =  optional tooltip

int CustomDialog::addColorSel(QString caption, QColor *color, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_COLOR, caption, tooltip, true);

  e.returnColor = color;
  e.btnColor = new ColorButton(*color,this);
  if(!tooltip.isEmpty())
    e.btnColor->setToolTip(tooltip);

  QSpacerItem *sp = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
  e.layout->addWidget(e.label);
  e.layout->addItem(sp);
  e.layout->addWidget(e.btnColor);
  layoutNextElement->addLayout(e.layout);

  return elements.size();
}

//------------------------
//-- Adds a two spin boxes to the next row of the dialog. These two spin boxes
//-- are prefixed by a label with the text "caption" and another label with
//-- the text "middleCpation" between them. Typically these two spin boxes
//-- represent a min and max value - although currently they are not inteligent
//-- enough to prevent the min value from being less than the max value.
//-- @ caption       =  the caption for the label to the left of the first spin box
//-- @ middleCaption =  the caption for the label placed between the two spin boxes
//-- @ min, max      =  minimum and maximum (integer) limits for both spin boxes
//-- @ *minValue     =  default value + where the first spin box value is saved if
//--                    user clicks "Ok"
//-- @ *maxValue     =  default value + where the first spin box value is saved if
//--                    user clicks "Ok"
//-- @ step          =  the interval both spin box advance when their arrows are clicked
//-- @ tooltip       =  optional tooltip for all the form elements in this row

int CustomDialog::addMinMaxSpinBoxPair(QString caption, QString middleCaption,
                                       int min, int max,
                                       int *minValue, int *maxValue, int step,
                                       QString tooltip)
{
  DialogElement &e = addNewElement(DLG_MINMAXSPIN, caption, tooltip, true);

  e.returnInt = minValue;
  e.spnBox = new QSpinBox(this);
  e.spnBox->setRange     (min, max);
  e.spnBox->setValue     (*minValue);
  e.spnBox->setSingleStep(step);

  e.returnInt2 = maxValue;
  e.spnBox2 = new QSpinBox(this);
  e.spnBox2->setRange     (min, max);
  e.spnBox2->setValue     (*maxValue);
  e.spnBox2->setSingleStep(step);

  e.label2 = new QLabel(middleCaption,this);

  if(!tooltip.isEmpty())
  {
    e.spnBox->setToolTip(tooltip);
    e.spnBox2->setToolTip(tooltip);
    e.label2->setToolTip(tooltip);
  }

  QSpacerItem *sp = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);

  e.layout->addWidget(e.label);
  e.layout->addItem(sp);
  e.layout->addWidget(e.spnBox);
  e.layout->addWidget(e.label2);
  e.layout->addWidget(e.spnBox2);
  layoutNextElement->addLayout(e.layout);

  return elements.size();
}



//------------------------
//-- Adds a text edit area (the type which automatically has a scroll bar)
//-- which may be configured as read only and/or to accept right text.
//-- @ *text     = the default text shown + where user text is saved if user clicks "Ok"
//-- @ readOnly  = number of decimal points to show
//-- @ acceptRichText = if true: HTML is supported for text... but also means output
//--                    will be (complex to analyze) HTML
//-- @ minHeight = the minimum height of the text area in pixels
//-- @ tooltip   =  optional tooltip for all the form elements in this row

int CustomDialog::addTextEdit(string *text, bool richText, bool readOnly,
                               int minHeight, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_TEXTEDIT, "", tooltip, false);

  e.returnString = text;
  e.textEdit = new QTextEdit(this);
  e.textEdit->setAcceptRichText(richText);
  if(richText)
    e.textEdit->setHtml(text->c_str());
  else
    e.textEdit->setPlainText(text->c_str());

  if(!tooltip.isEmpty())
    e.textEdit->setToolTip(tooltip);
  e.textEdit->setReadOnly(readOnly);
  e.textEdit->setMinimumHeight(minHeight);

  e.layout->addWidget(e.textEdit);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a read only text edit area (the type which automatically has a scroll bar)
//-- which may be configured as read only and/or to accept right text.
//-- @ *text     = the default text shown + where user text is saved if user clicks "Ok"
//-- @ acceptRichText = if true: HTML is supported for text... but also means output
//--                    will be (complex to analyze) HTML
//-- @ minHeight = the minimum height of the text area in pixels
//-- @ tooltip   =  optional tooltip for all the form elements in this row

int CustomDialog::addReadOnlyTextEdit(QString text, bool richText,
                                       int minHeight, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_TEXTEDIT, "", tooltip, false);

  e.textEdit = new QTextEdit(this);
  e.textEdit->setAcceptRichText(richText);
  if(richText)
    e.textEdit->setHtml(text);
  else
    e.textEdit->setPlainText(text);

  if(!tooltip.isEmpty())
    e.textEdit->setToolTip(tooltip);
  e.textEdit->setReadOnly(true);
  e.textEdit->setMinimumHeight(minHeight);

  e.layout->addWidget(e.textEdit);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a plain text label plus a progress bar to the next row of the dialog
//-- @ caption    = the caption for the label
//-- @ percent    = the percentage value as an integer
//-- @ width      = the fixed width of the progress bar in pixels
//-- @ showValue  = if true: the percentage value will appear in black, else is invisible
//-- @ tooltip    =  optional tooltip for all the form elements in this row

int CustomDialog::addProgressBar(QString caption, int percent,
                                  int width, bool showValue, QString tooltip)
{
  DialogElement &e = addNewElement(DLG_LABEL, caption, tooltip, true);

  QProgressBar *progBar = new QProgressBar(this);
  progBar->setMinimum(0);
  progBar->setMaximum(100);
  progBar->setValue(percent);
  progBar->setFixedWidth(width);
  if(showValue)
    progBar->setStyleSheet("color: rgb(0,0,0);");    // Black.
  if(!tooltip.isEmpty())
    progBar->setToolTip(tooltip);

  QSpacerItem *sp = new QSpacerItem(10,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
  e.layout->addWidget(e.label);
  e.layout->addItem(sp);
  e.layout->addWidget(progBar);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}


//------------------------
//-- Adds a plain text label plus a progress bar to the next row of the dialog
//-- @ caption    = the caption for the label
//-- @ fraction   = the length of the bar as a fraction of the "width"
//-- @ width      = the fixed width of the progress bar in pixels
//-- @ showValue  = if true: the percentage value will appear in black, else is invisible
//-- @ tooltip    =  optional tooltip for all the form elements in this row

int CustomDialog::addPercentBar(QString caption, QString valueLabel, float fraction,
                                 int width, QColor colorBar, QString tooltip,
                                 QFrame::Shape shape, QFrame::Shadow shadow)
{
  DialogElement &e = addNewElement(DLG_LABEL, caption, tooltip, true);

  QSpacerItem *sp = new QSpacerItem(10,20,QSizePolicy::Expanding,QSizePolicy::Minimum);

  e.label2 = new QLabel(valueLabel,this);

  QSpacerItem *sp2 = new QSpacerItem(10,20,QSizePolicy::Minimum,QSizePolicy::Minimum);

  int widthLine1 = (int)(fraction * (float)width);
  int widthLine2 = width - widthLine1;

  QFrame *fraBar = new QFrame(this);
  fraBar->setFrameShape(shape);
  fraBar->setFrameShadow(shadow);

  QHBoxLayout *layLines = new QHBoxLayout();
  layLines->setSpacing(0);
  layLines->setMargin(0);
  fraBar->setLayout(layLines);

  QFrame *line1 = new QFrame(this);
  line1->setObjectName(QString::fromUtf8("line1"));
  line1->setFixedWidth(widthLine1);
  line1->setMinimumHeight(18);
  line1->setFrameShape(QFrame::HLine);
  line1->setFrameShadow(QFrame::Plain);
  line1->setLineWidth(15);
  line1->setStyleSheet("color: " + colorBar.name() + ";");
  layLines->addWidget(line1);

  QFrame *line2 = new QFrame(this);
  line2->setObjectName(QString::fromUtf8("line1"));
  line2->setFixedWidth(widthLine2);
  line2->setMinimumHeight(18);
  line2->setLineWidth(15);
  line2->setFrameShape(QFrame::HLine);
  line2->setFrameShadow(QFrame::Plain);
  line2->setStyleSheet("color: rgb(255,255,255,80);");
  layLines->addWidget(line2);

  if(!tooltip.isEmpty())
  {
    line1->setToolTip(tooltip);
    line2->setToolTip(tooltip);
  }

  e.layout->addWidget(e.label);
  e.layout->addItem(sp);
  e.layout->addWidget(e.label2);
  e.layout->addItem(sp2);
  //e.layout->addLayout(layLines);
  e.layout->addWidget(fraBar);
  layoutNextElement->addLayout(e.layout);
  return elements.size();
}

//------------------------
//-- Adds a vertical spacer below the last added dialog element.
//-- @ minHeight  = the minimum height for the vertical spacer

int CustomDialog::addVSpacer(int minHeight)
{
  QSpacerItem *sp = new QSpacerItem(0, minHeight,
                                    QSizePolicy::Minimum, QSizePolicy::Expanding);

  //e.layout->addItem(sp);
  layoutNextElement->addItem(sp);
  return elements.size();
}


//------------------------
//-- Begins a new group box to the next row of the dialog. All new elements added after
//-- this will be placed inside this box. "endGroupBox" must be called to end the box.
//-- @ caption    = the caption/title of the group box
//-- @ flat       = if true, the group box area won't have a box drawn around it
//-- @ tooltip    = optional tooltip applied to group box
//-- @ checkable  = wether or not a checkbox appears in the title of the group box.
//-- @ *checked   = if "checkable" is true, the default value + where the value of the
//--                checkbox is updated when/if the user clicks "Ok"

int CustomDialog::beginGroupBox(QString caption, bool flat, QString tooltip,
                                bool checkable, bool *checked)
{
  DialogElement &e = addNewElement(DLG_GRPBOX, caption, tooltip, false);

  e.grpBox = new QGroupBox(this);
  e.grpBox->setTitle(caption);
  e.grpBox->setFlat(flat);
  if(checkable)
  {
    e.returnBool = checked;
    e.grpBox->setCheckable(*checked);
  }

  groupBoxLayout = new QVBoxLayout(e.grpBox);
  groupBoxLayout->setSpacing(2);
  groupBoxLayout->setContentsMargins(8, 12, 5, 5);
  if(!tooltip.isEmpty())
    e.grpBox->setToolTip(tooltip);

  e.layout->addWidget(e.grpBox);
  layoutNextElement->addLayout(e.layout);

  layoutNextElement = groupBoxLayout;

  //e.grpBox->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(220, 233, 255, 255));");  // nice background color

  return elements.size();
}

//------------------------
//-- Must be called AFTER "beginGroupBox" and ends the group box

void CustomDialog::endGroupBox()
{
  layoutNextElement = vboxLayout;
}

//------------------------
//-- Adds an extra checkbox into to the previously added element which (depending on the
//-- value of "chkBehav" can be used to enable, disable, show or hide that element when it
//-- is ticked. For most elements (DLG_SPINBOX, DLG_DBLSPINBOX, DLG_COLOR, DLG_FLOATEDIT)
//-- the checkbox is added in the middle of that row, after the label. For a DLG_RADIOGRP
//-- or DLG_GRPBOX, the checkbox is added to a row before the group box.
//--
//-- @ caption     = the caption for the little checkbox
//-- @ *checked    = the default value + where the value of the little checkbox
//--                is updated when/if the user clicks "Ok"
//-- @ chkbehavior = whether this little checkbox will do nothing (CB_NONE),
//--                  disable (CB_DISABLE), enable (CB_ENABLE), hide (CB_HIDE) or
//--                  show (CB_SHOW) the previous element when checked.
//-- @ removeLabel = will remove the little label added on the left by "addNewElement()"
//-- @ tooltip     = optional tooltip applied to the little checkbox

int CustomDialog::addCheckPrev(QString caption, bool *checked,
                                chkbehav chkBehav, bool removeLabel, QString tooltip)
{
  DialogElement &e = elements.back();
  e.extraChkAdded = true;

  e.returnChkExtra = checked;
  e.chkExtra = new QCheckBox(caption, this);
  e.chkExtra->setChecked(*checked);
  if(!tooltip.isEmpty())
    e.chkExtra->setToolTip(tooltip);

  //## INSERT CHECK BOX INTO CORRECT POSITION:

  int insertPos = 1;
  if(!removeLabel && (e.type==DLG_SPINBOX  || e.type==DLG_DBLSPINBOX
                      || e.type==DLG_MINMAXSPIN || e.type==DLG_COLOR
                      || e.type==DLG_FLOATEDIT))
    insertPos = 2;
  if(e.type==DLG_RADIOGRP || e.type==DLG_GRPBOX)
    vboxLayout->insertWidget(vboxLayout->count()-1, e.chkExtra);
  else
    e.layout->insertWidget(insertPos, e.chkExtra);

  //## REMOVE LABEL IF SPECIFIED:

  if(removeLabel && e.label)
  {
    e.layout->removeWidget(e.label);
    e.label->setParent(0);
  }

  //## ADD APPROPRIATE CONNECTOR TO APPROPRIATE WIDGET:

  QWidget *wid;

  switch(e.type)
  {
  case(DLG_LABEL):      wid = e.label;      break;
  case(DLG_CHECKBOX):   wid = e.chkBox;     break;
  case(DLG_LINEEDIT):   wid = e.lineEdit;   break;
  case(DLG_FLOATEDIT):  wid = e.lineEdit;   break;
  case(DLG_SPINBOX):    wid = e.spnBox;     break;
  case(DLG_DBLSPINBOX): wid = e.dblSpnBox;  break;
  case(DLG_COMBOBOX):   wid = e.cmbBox;     break;
  case(DLG_RADIOGRP):   wid = e.grpBox;     break;
  case(DLG_GRPBOX):     wid = e.grpBox;     break;
  case(DLG_COLOR):      wid = e.btnColor;   break;
  case(DLG_TEXTEDIT):   wid = e.textEdit;   break;
  }

  switch(chkBehav)
  {
  case(CB_NONE): break;
  case(CB_ENABLE):
    wid->setEnabled(*checked);
    e.chkExtra->connect(e.chkExtra, SIGNAL(clicked(bool)), wid, SLOT(setEnabled(bool)));
    break;
  case(CB_DISABLE):
    wid->setDisabled(*checked);
    e.chkExtra->connect(e.chkExtra, SIGNAL(clicked(bool)), wid, SLOT(setDisabled(bool)));
    break;
  case(CB_SHOW):
    wid->setVisible(*checked);
    e.chkExtra->connect(e.chkExtra, SIGNAL(clicked(bool)), wid, SLOT(setVisible(bool)));
    e.chkExtra->connect(this, SIGNAL(clicked(bool)), wid, SLOT(reize()));
    connect(e.chkExtra, SIGNAL(clicked()), this, SLOT(resizeMe()));
    break;
  case(CB_HIDE):
    wid->setHidden(*checked);
    e.chkExtra->connect(e.chkExtra, SIGNAL(clicked(bool)), wid, SLOT(setHidden(bool)));
    e.chkExtra->connect(this, SIGNAL(clicked(bool)), wid, SLOT(reize()));
    connect(e.chkExtra, SIGNAL(clicked()), this, SLOT(resizeMe()));
    break;
  }

  return elements.size();
}



//------------------------
//-- Adds an autocomplete feature to the previous added element, where the previously
//-- added element must be a line edit box.
//-- Will return the index of the element or -1 if the element was not a line edit.
//--
//-- @ wordList      = the list of words that comprise the auto complete
//-- @ caseSensitive = if true, the autocomplete is case sensitive

int CustomDialog::addAutoCompletePrev(QStringList wordList, bool caseSensitive)
{
  DialogElement &e = elements.back();
  if (e.type != DLG_LINEEDIT)
  {
    cerr << "ERROR: addAutoCompletePrev() must proceed addLineEdit()" << endl;
    return 01;
  }

  QCompleter *completer = new QCompleter(wordList, this);
  if(caseSensitive)
    completer->setCaseSensitivity(Qt::CaseSensitive);
  else
    completer->setCaseSensitivity(Qt::CaseInsensitive);
  e.lineEdit->setCompleter(completer);

  return elements.size();
}


//------------------------
//-- Sets the style sheet to the element at the given index in the elements vector.
//-- Returns true if successful, or false if element doesn't exist.
//-- @ idx     = the index of the element to apply the stylesheet to
//-- @ syleStr = the style sheet to apply
//--             eg: "color: rgb(0, 0, 0); background-color: rgba(255, 255, 255, 0);"
//-- @ bold    = if true: the font for the element will be made bold

bool CustomDialog::setStyleElem(int idx, string styleStr, bool bold)
{
  if(idx >= elements.size() || idx < 0)
    return false;

  DialogElement &e = elements[idx];
  QString styleQStr = (QString)styleStr.c_str();


  if(e.extraChkAdded)
    e.chkExtra->setStyleSheet(styleQStr);

  if(e.type == DLG_LABEL || e.type == DLG_LINEEDIT || e.type == DLG_FLOATEDIT
     || e.type == DLG_SPINBOX || e.type == DLG_DBLSPINBOX || e.type == DLG_COMBOBOX
     || e.type == DLG_COLOR)
    e.label->setStyleSheet(styleQStr);

  switch (e.type)
  {
    case(DLG_CHECKBOX  ):    e.chkBox->setStyleSheet(styleQStr);     break;
    case(DLG_LINEEDIT  ):
    case(DLG_FLOATEDIT ):    e.lineEdit->setStyleSheet(styleQStr);   break;
    case(DLG_SPINBOX   ):    e.spnBox->setStyleSheet(styleQStr);     break;
    case(DLG_DBLSPINBOX):    e.dblSpnBox->setStyleSheet(styleQStr);  break;
    case(DLG_COMBOBOX  ):    e.cmbBox->setStyleSheet(styleQStr);     break;
    case(DLG_RADIOGRP  ):
    {
      for(int i=0; i<(int)e.radBtn.size(); i++)
        e.radBtn[i]->setStyleSheet(styleQStr);
    } break;
    case(DLG_COLOR     ):    break;
    case(DLG_GRPBOX    ):    e.grpBox->setStyleSheet(styleQStr);     break;
    case(DLG_TEXTEDIT  ):    e.textEdit->setStyleSheet(styleQStr);     break;
  }

  return true;
}


//------------------------
//-- Sets the style sheet for the previously added element
//-- @ syleStr = the style sheet to apply
//--             eg: "color: rgb(0, 0, 0); background-color: rgba(255, 255, 255, 0);"
//-- @ bold    = if true: the font for the element will be made bold

void CustomDialog::setStylePrev(string styleStr, bool bold)
{
  setStyleElem(elements.size()-1, styleStr, bold);
}


//------------------------
//-- Used to disable or enable the element at the given index in the element vector.
//-- Returns true if successful, or false if element doesn't exist.
//-- @ idx     = the index of the element to apply the stylesheet to
//-- @ enabled = if true: will enable, if false: will disable

bool CustomDialog::setEnabledElem(int idx, bool enabled)
{
  if(idx >= elements.size() || idx < 0)
    return false;

  DialogElement &e = elements[idx];
  e.layout->setEnabled(enabled);
}


//------------------------
//-- Used to disable or enable the previously added element.
//-- @ enabled = if true: will enable, if false: will disable

void CustomDialog::setEnabledPrev(bool enabled)
{
  setEnabledElem(elements.size()-1, enabled);
}

//------------------------
//-- Used to disable or enable the all element currently added.
//-- @ enabled = if true: will enable, if false: will disable

void CustomDialog::setEnabledAll(bool enabled)
{
  for(int i=0; i<(int)elements.size(); i++)
    setEnabledElem(i, enabled);
}



//------------------------
//-- Resizes the dialog to the minimum size possible.

void CustomDialog::resizeMe()
{
  this->resize(20,20);
  adjustSize();
}

//------------------------
//-- Called when the user clicks "Okay" or any other button set as "BB_ACCEPT".
//-- The first thing this function does is update the value of "customBtnClicked".
//-- More importantly, this function takes the values from each dialog element
//-- int "elements" and updates the appropriate variable.
//-- For example if you called: "dlg.addLineEdit("name:",&name);"... and your user
//-- types "Bloggs", the value at the location "&name" changes to "Bloggs".
//-- After updating values, "QDialog::accept()" is called so the dialog closes.

void CustomDialog::customBtnAccept()
{
  updateBtnClicked(QObject::sender());    // updates the value of "customBtnClicked"

  for(int i = 0; i<(int)elements.size(); i++)
  {
    DialogElement &e = elements[i];

    if(e.extraChkAdded)
      *e.returnChkExtra = e.chkExtra->isChecked();

    switch (e.type)
    {
    case(DLG_LABEL): break;
    case(DLG_CHECKBOX):
      *e.returnBool = e.chkBox->isChecked();
      break;

    case(DLG_LINEEDIT):
      *e.returnString = e.lineEdit->text().toStdString();
      break;

    case(DLG_FLOATEDIT):
      *e.returnFloat = e.lineEdit->text().toFloat();
      break;

    case(DLG_SPINBOX):
      *e.returnInt = (int)e.spnBox->value();
      break;

    case(DLG_DBLSPINBOX):
      *e.returnFloat = (float)e.dblSpnBox->value();
      break;

    case(DLG_COMBOBOX):
      *e.returnInt = (double)e.cmbBox->currentIndex();
      break;

    case(DLG_RADIOGRP):
      {
        *e.returnInt = 0;
        for(int j=0; j<(int)e.radBtn.size(); j++)
          if(e.radBtn[j]->isChecked())
            *e.returnInt = j;
        if(e.grpBox->isCheckable())
          *e.returnInt = e.grpBox->isChecked();
      }
      break;

    case(DLG_COLOR):
      *e.returnColor = e.btnColor->color;
      break;

    case(DLG_MINMAXSPIN):
        *e.returnInt = (int)e.spnBox->value();
        *e.returnInt2 = (int)e.spnBox2->value();
        break;

    case(DLG_GRPBOX):
      if(e.grpBox->isCheckable())
        *e.returnInt = e.grpBox->isChecked();
      break;

    case(DLG_TEXTEDIT):
      if(e.textEdit->isReadOnly() == false)
        *e.returnString = e.textEdit->toPlainText().toStdString();
      break;
    }

  }

  QDialog::accept();
}

//------------------------
//-- Called when the user clicks "Cancel" or any other button set as "BB_REJECT".
//-- This function updates the value of "customBtnClicked" and then calls
//-- "QDialog::reject()", WITHOUT updating/changing any of the reference values
//-- passed into the plugin.

void CustomDialog::customBtnReject()
{
  updateBtnClicked(QObject::sender());    // updates the value of "customBtnClicked"
  QDialog::reject();
}

//------------------------
//-- Called when user clicks a custom buttom created with "addCustomButton()".
//-- Saves the index of the button clicked to "customBtnClicked".

void CustomDialog::updateBtnClicked(QObject* btnClicked)
{
  customBtnClicked = -1;
  for(int i=0; i<(int)customBtn.size(); i++)
    if(btnClicked==customBtn[i])
      customBtnClicked = i;
}

//------------------------
//-- Called when user clicks a custom buttom created with "addCustomButton()".
//-- Displays a popup containing the same text as the button's tooltip.

void CustomDialog::customBtnMessage()
{
  for(int i=0; i<(int)customBtn.size(); i++)
    if(QObject::sender()==customBtn[i])
      MsgBox(customBtn[i], "...", customBtn[i]->toolTip());
}

//------------------------
//-- Called when user clicks a custom buttom created with "addCustomButton()".
//-- Opens a web browser using the same text as the button's tooltip as the URL.

void CustomDialog::customBtnOpenUrl()
{
  for(int i=0; i<(int)customBtn.size(); i++)
    if(QObject::sender()==customBtn[i])
      openUrl(customBtn[i]->toolTip());
}


//------------------------
//-- Begins execution of and shows the dialog you created.

int CustomDialog::exec()
{
  adjustSize();
  return QDialog::exec();
}

//------------------------
//-- Simple accessor which returns true if the user hit "Cancel"
//-- (and thus the input values were not changed) or false if the user hit "Ok"
//-- (and thus some of the input values may have changed).

bool CustomDialog::wasCancelled()
{
  return (result() == QDialog::Rejected);
}


//############################################################




//----------------------------------------------------------------------------
//
//          SMALL MESSAGE BOX FUNCTIONS:
//
//----------------------------------------------------------------------------


//---------
//-- Display a simple message box

void MsgBox(string str)
{
  QMessageBox::information(0, "...", str.c_str());
}

//---------
//-- Display a simple message box

void MsgBox(QWidget *parent, QString title, QString str)
{
  QMessageBox::information(parent, title, str);
}

//---------
//-- Display a tyes/no dialog box and return "true" if use clicks yes.

bool MsgBoxYesNo(QWidget *parent, string str)
{
  int result = QMessageBox::information(parent, "...", str.c_str(),
                                        QMessageBox::Yes, QMessageBox::No);
  return (result == QMessageBox::Yes);
}

//---------
//-- Display an input dialog and return the string entered by the user.

string InputBoxString(QWidget *parent, string title, string label, string defaultStr)
{
  return qStringToString(QInputDialog::getText(parent, title.c_str(), label.c_str(),
                                               QLineEdit::Normal, defaultStr.c_str()));
}


//############################################################
