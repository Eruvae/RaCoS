/****************************************************************************
** Meta object code from reading C++ file 'qneedleindicator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtGSInteface/qneedleindicator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qneedleindicator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QNeedleIndicator_t {
    QByteArrayData data[15];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QNeedleIndicator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QNeedleIndicator_t qt_meta_stringdata_QNeedleIndicator = {
    {
QT_MOC_LITERAL(0, 0, 16), // "QNeedleIndicator"
QT_MOC_LITERAL(1, 17, 7), // "animate"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "setValue"
QT_MOC_LITERAL(4, 35, 5), // "value"
QT_MOC_LITERAL(5, 41, 3), // "min"
QT_MOC_LITERAL(6, 45, 3), // "max"
QT_MOC_LITERAL(7, 49, 9), // "gap_angle"
QT_MOC_LITERAL(8, 59, 8), // "animated"
QT_MOC_LITERAL(9, 68, 10), // "majorTicks"
QT_MOC_LITERAL(10, 79, 10), // "minorTicks"
QT_MOC_LITERAL(11, 90, 9), // "labelFont"
QT_MOC_LITERAL(12, 100, 9), // "digitFont"
QT_MOC_LITERAL(13, 110, 5), // "label"
QT_MOC_LITERAL(14, 116, 11) // "labelOffset"

    },
    "QNeedleIndicator\0animate\0\0setValue\0"
    "value\0min\0max\0gap_angle\0animated\0"
    "majorTicks\0minorTicks\0labelFont\0"
    "digitFont\0label\0labelOffset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QNeedleIndicator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
      11,   28, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    1,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,    4,

 // properties: name, type, flags
       5, QMetaType::QReal, 0x00095003,
       6, QMetaType::QReal, 0x00095003,
       4, QMetaType::QReal, 0x00095103,
       7, QMetaType::QReal, 0x00095003,
       8, QMetaType::Bool, 0x00095103,
       9, QMetaType::Int, 0x00095103,
      10, QMetaType::Int, 0x00095103,
      11, QMetaType::QFont, 0x00095103,
      12, QMetaType::QFont, 0x00095103,
      13, QMetaType::QString, 0x00095103,
      14, QMetaType::QReal, 0x00095103,

       0        // eod
};

void QNeedleIndicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QNeedleIndicator *_t = static_cast<QNeedleIndicator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->animate(); break;
        case 1: _t->setValue((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        QNeedleIndicator *_t = static_cast<QNeedleIndicator *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->getMinValue(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = _t->getMaxValue(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = _t->getValue(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = _t->getGapAngle(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->isAnimated(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->getMajorTicks(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->getMinorTicks(); break;
        case 7: *reinterpret_cast< QFont*>(_v) = _t->getLabelFont(); break;
        case 8: *reinterpret_cast< QFont*>(_v) = _t->getDigitFont(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->getLabel(); break;
        case 10: *reinterpret_cast< qreal*>(_v) = _t->getLabelOffset(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        QNeedleIndicator *_t = static_cast<QNeedleIndicator *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMinValue(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setMaxValue(*reinterpret_cast< qreal*>(_v)); break;
        case 2: _t->setValue(*reinterpret_cast< qreal*>(_v)); break;
        case 3: _t->setGapAngle(*reinterpret_cast< qreal*>(_v)); break;
        case 4: _t->setAnimated(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setMajorTicks(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setMinorTicks(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setLabelFont(*reinterpret_cast< QFont*>(_v)); break;
        case 8: _t->setDigitFont(*reinterpret_cast< QFont*>(_v)); break;
        case 9: _t->setLabel(*reinterpret_cast< QString*>(_v)); break;
        case 10: _t->setLabelOffset(*reinterpret_cast< qreal*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QNeedleIndicator::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QNeedleIndicator.data,
      qt_meta_data_QNeedleIndicator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QNeedleIndicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QNeedleIndicator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QNeedleIndicator.stringdata0))
        return static_cast<void*>(const_cast< QNeedleIndicator*>(this));
    return QWidget::qt_metacast(_clname);
}

int QNeedleIndicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
