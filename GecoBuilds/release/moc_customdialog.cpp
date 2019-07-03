/****************************************************************************
** Meta object code from reading C++ file 'customdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Headers/customdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ColorButton_t {
    QByteArrayData data[3];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ColorButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ColorButton_t qt_meta_stringdata_ColorButton = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ColorButton"
QT_MOC_LITERAL(1, 12, 9), // "pickColor"
QT_MOC_LITERAL(2, 22, 0) // ""

    },
    "ColorButton\0pickColor\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ColorButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ColorButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColorButton *_t = static_cast<ColorButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pickColor(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ColorButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_ColorButton.data,
      qt_meta_data_ColorButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ColorButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ColorButton.stringdata0))
        return static_cast<void*>(const_cast< ColorButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int ColorButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_CustomDialog_t {
    QByteArrayData data[10];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CustomDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CustomDialog_t qt_meta_stringdata_CustomDialog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CustomDialog"
QT_MOC_LITERAL(1, 13, 15), // "customBtnAccept"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 15), // "customBtnReject"
QT_MOC_LITERAL(4, 46, 16), // "customBtnMessage"
QT_MOC_LITERAL(5, 63, 16), // "customBtnOpenUrl"
QT_MOC_LITERAL(6, 80, 16), // "updateBtnClicked"
QT_MOC_LITERAL(7, 97, 10), // "btnClicked"
QT_MOC_LITERAL(8, 108, 8), // "resizeMe"
QT_MOC_LITERAL(9, 117, 4) // "exec"

    },
    "CustomDialog\0customBtnAccept\0\0"
    "customBtnReject\0customBtnMessage\0"
    "customBtnOpenUrl\0updateBtnClicked\0"
    "btnClicked\0resizeMe\0exec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CustomDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    1,   53,    2, 0x0a /* Public */,
       8,    0,   56,    2, 0x0a /* Public */,
       9,    0,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,    7,
    QMetaType::Void,
    QMetaType::Int,

       0        // eod
};

void CustomDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CustomDialog *_t = static_cast<CustomDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->customBtnAccept(); break;
        case 1: _t->customBtnReject(); break;
        case 2: _t->customBtnMessage(); break;
        case 3: _t->customBtnOpenUrl(); break;
        case 4: _t->updateBtnClicked((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 5: _t->resizeMe(); break;
        case 6: { int _r = _t->exec();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CustomDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CustomDialog.data,
      qt_meta_data_CustomDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CustomDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CustomDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CustomDialog.stringdata0))
        return static_cast<void*>(const_cast< CustomDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CustomDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
