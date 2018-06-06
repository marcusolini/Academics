/****************************************************************************
** Meta object code from reading C++ file 'leakdialogqtui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../leakdialogqtui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'leakdialogqtui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LeakDialogQtUi_t {
    QByteArrayData data[6];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LeakDialogQtUi_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LeakDialogQtUi_t qt_meta_stringdata_LeakDialogQtUi = {
    {
QT_MOC_LITERAL(0, 0, 14), // "LeakDialogQtUi"
QT_MOC_LITERAL(1, 15, 17), // "handleCloseButton"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 25), // "handleNewOperatorCheckBox"
QT_MOC_LITERAL(4, 60, 5), // "state"
QT_MOC_LITERAL(5, 66, 20) // "handleRunTestsButton"

    },
    "LeakDialogQtUi\0handleCloseButton\0\0"
    "handleNewOperatorCheckBox\0state\0"
    "handleRunTestsButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LeakDialogQtUi[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,

       0        // eod
};

void LeakDialogQtUi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LeakDialogQtUi *_t = static_cast<LeakDialogQtUi *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleCloseButton(); break;
        case 1: _t->handleNewOperatorCheckBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->handleRunTestsButton(); break;
        default: ;
        }
    }
}

const QMetaObject LeakDialogQtUi::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LeakDialogQtUi.data,
      qt_meta_data_LeakDialogQtUi,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LeakDialogQtUi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LeakDialogQtUi::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LeakDialogQtUi.stringdata0))
        return static_cast<void*>(const_cast< LeakDialogQtUi*>(this));
    return QDialog::qt_metacast(_clname);
}

int LeakDialogQtUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
