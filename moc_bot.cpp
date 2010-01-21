/****************************************************************************
** Meta object code from reading C++ file 'bot.h'
**
** Created: Thu Jan 21 13:06:52 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "bot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Bot[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      19,    4,    4,    4, 0x08,
      31,    4,    4,    4, 0x08,
      69,    4,    4,    4, 0x08,
      92,   88,   80,    4, 0x4a,

       0        // eod
};

static const char qt_meta_stringdata_Bot[] = {
    "Bot\0\0onReadReady()\0onConnect()\0"
    "onError(QAbstractSocket::SocketError)\0"
    "sendHelp()\0QString\0arg\0commit(QString)\0"
};

const QMetaObject Bot::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Bot,
      qt_meta_data_Bot, 0 }
};

const QMetaObject *Bot::metaObject() const
{
    return &staticMetaObject;
}

void *Bot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Bot))
        return static_cast<void*>(const_cast< Bot*>(this));
    return QObject::qt_metacast(_clname);
}

int Bot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onReadReady(); break;
        case 1: onConnect(); break;
        case 2: onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: sendHelp(); break;
        case 4: { QString _r = commit((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
