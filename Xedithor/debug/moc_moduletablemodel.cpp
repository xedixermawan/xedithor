/****************************************************************************
** Meta object code from reading C++ file 'moduletablemodel.h'
**
** Created: Sun Dec 11 14:12:46 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/moduletablemodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'moduletablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModuleTableModel[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ModuleTableModel[] = {
    "ModuleTableModel\0\0editCompleted(QString)\0"
    "timerHit()\0"
};

const QMetaObject ModuleTableModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_ModuleTableModel,
      qt_meta_data_ModuleTableModel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModuleTableModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModuleTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModuleTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModuleTableModel))
        return static_cast<void*>(const_cast< ModuleTableModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int ModuleTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: editCompleted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: timerHit(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ModuleTableModel::editCompleted(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE