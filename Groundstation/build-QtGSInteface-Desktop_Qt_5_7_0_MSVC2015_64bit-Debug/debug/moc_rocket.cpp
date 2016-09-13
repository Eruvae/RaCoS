/****************************************************************************
** Meta object code from reading C++ file 'rocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtGSInteface/rocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Rocket_t {
    QByteArrayData data[5];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Rocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Rocket_t qt_meta_stringdata_Rocket = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Rocket"
QT_MOC_LITERAL(1, 7, 12), // "thetaChanged"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 5), // "theta"
QT_MOC_LITERAL(4, 27, 8) // "setTheta"

    },
    "Rocket\0thetaChanged\0\0theta\0setTheta"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Rocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       1,   30, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    3,

 // properties: name, type, flags
       3, QMetaType::Float, 0x00495103,

 // properties: notify_signal_id
       0,

       0        // eod
};

void Rocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Rocket *_t = static_cast<Rocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->thetaChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->setTheta((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Rocket::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Rocket::thetaChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Rocket *_t = static_cast<Rocket *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = _t->theta(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Rocket *_t = static_cast<Rocket *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTheta(*reinterpret_cast< float*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Rocket::staticMetaObject = {
    { &Qt3DCore::QEntity::staticMetaObject, qt_meta_stringdata_Rocket.data,
      qt_meta_data_Rocket,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Rocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Rocket::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Rocket.stringdata0))
        return static_cast<void*>(const_cast< Rocket*>(this));
    return Qt3DCore::QEntity::qt_metacast(_clname);
}

int Rocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Qt3DCore::QEntity::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Rocket::thetaChanged(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
