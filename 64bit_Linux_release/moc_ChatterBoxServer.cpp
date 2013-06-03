/****************************************************************************
** Meta object code from reading C++ file 'ChatterBoxServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ChatterBoxServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChatterBoxServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ChatterBoxServer_t {
    QByteArrayData data[8];
    char stringdata[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ChatterBoxServer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ChatterBoxServer_t qt_meta_stringdata_ChatterBoxServer = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 9),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 12),
QT_MOC_LITERAL(4, 41, 12),
QT_MOC_LITERAL(5, 54, 4),
QT_MOC_LITERAL(6, 59, 10),
QT_MOC_LITERAL(7, 70, 28)
    },
    "ChatterBoxServer\0readyRead\0\0disconnected\0"
    "sendUserList\0kick\0checkError\0"
    "QAbstractSocket::SocketError\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatterBoxServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08,
       3,    0,   40,    2, 0x08,
       4,    0,   41,    2, 0x08,
       5,    0,   42,    2, 0x08,
       6,    1,   43,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,

       0        // eod
};

void ChatterBoxServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChatterBoxServer *_t = static_cast<ChatterBoxServer *>(_o);
        switch (_id) {
        case 0: _t->readyRead(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->sendUserList(); break;
        case 3: _t->kick(); break;
        case 4: _t->checkError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject ChatterBoxServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_ChatterBoxServer.data,
      qt_meta_data_ChatterBoxServer,  qt_static_metacall, 0, 0}
};


const QMetaObject *ChatterBoxServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatterBoxServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChatterBoxServer.stringdata))
        return static_cast<void*>(const_cast< ChatterBoxServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int ChatterBoxServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
