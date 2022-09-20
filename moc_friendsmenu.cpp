/****************************************************************************
** Meta object code from reading C++ file 'friendsmenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "friendsmenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'friendsmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FriendsMenu_t {
    QByteArrayData data[14];
    char stringdata0[188];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FriendsMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FriendsMenu_t qt_meta_stringdata_FriendsMenu = {
    {
QT_MOC_LITERAL(0, 0, 11), // "FriendsMenu"
QT_MOC_LITERAL(1, 12, 14), // "connectToLobby"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "string"
QT_MOC_LITERAL(4, 35, 9), // "lobbyName"
QT_MOC_LITERAL(5, 45, 15), // "addFriendToList"
QT_MOC_LITERAL(6, 61, 18), // "removeFriendToList"
QT_MOC_LITERAL(7, 80, 19), // "acceptFriendRequest"
QT_MOC_LITERAL(8, 100, 20), // "declineFriendRequest"
QT_MOC_LITERAL(9, 121, 21), // "removeRequestFromList"
QT_MOC_LITERAL(10, 143, 7), // "request"
QT_MOC_LITERAL(11, 151, 9), // "joinLobby"
QT_MOC_LITERAL(12, 161, 13), // "clickedFriend"
QT_MOC_LITERAL(13, 175, 12) // "writeMessage"

    },
    "FriendsMenu\0connectToLobby\0\0string\0"
    "lobbyName\0addFriendToList\0removeFriendToList\0"
    "acceptFriendRequest\0declineFriendRequest\0"
    "removeRequestFromList\0request\0joinLobby\0"
    "clickedFriend\0writeMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FriendsMenu[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    1,   66,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,
      12,    0,   70,    2, 0x08 /* Private */,
      13,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FriendsMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FriendsMenu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectToLobby((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 1: _t->addFriendToList(); break;
        case 2: _t->removeFriendToList(); break;
        case 3: _t->acceptFriendRequest(); break;
        case 4: _t->declineFriendRequest(); break;
        case 5: _t->removeRequestFromList((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 6: _t->joinLobby(); break;
        case 7: _t->clickedFriend(); break;
        case 8: _t->writeMessage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FriendsMenu::*)(string );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FriendsMenu::connectToLobby)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FriendsMenu::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_FriendsMenu.data,
    qt_meta_data_FriendsMenu,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FriendsMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendsMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FriendsMenu.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Subject"))
        return static_cast< Subject*>(this);
    return QWidget::qt_metacast(_clname);
}

int FriendsMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FriendsMenu::connectToLobby(string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
