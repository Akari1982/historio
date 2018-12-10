#ifndef EVENT_H
#define EVENT_H

// add this include because this is only common file between everything that uses Input function.
#include <OIS.h>



enum EventType
{
    ET_NULL = 0,
    ET_PRESS,
    ET_REPEAT_WAIT,
    ET_REPEAT,
    ET_RELEASE,
    ET_MOUSE_MOVE,
    ET_MOUSE_SCROLL
};



struct Event
{
    Event():
        type( ET_NULL ),
        button( 0 ),
        param1( 0 ),
        param2( 0 ),
        param3( 0 ),
        param4( 0 ),
        event( "" )
    {
    };

    EventType type;
    int button;
    float param1;
    float param2;
    float param3;
    float param4;
    Ogre::String event;
};



#endif // EVENT_H
