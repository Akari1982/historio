#ifndef INPUT_FILTER_H
#define INPUT_FILTER_H

#include <OgreSingleton.h>
#include <OgreString.h>
#include <OIS.h>
#include <vector>

#include "Event.h"
#include "ConfigCmdManager.h"



typedef std::vector< Event > InputEventArray;
typedef std::vector< int > ButtonList;



class InputManager : public Ogre::Singleton< InputManager >
{
public:
                        InputManager();
    virtual             ~InputManager();

    void                ButtonPressed( const int button, const char text, const bool down );
    void                MousePressed( const int x, const int x_abs, const int y, const int y_abs, const int button, const bool down );
    void                MouseMoved( const int x, const int x_abs, const int y, const int y_abs );
    void                MouseScrolled( const int value );
    void                Reset();
    void                Update();

    bool                IsButtonPressed( const int button ) const;

    void                GetInputEvents( InputEventArray& input_events );

    // binds
    void                InitCmd();
    void                BindCommand( ConfigCmd* cmd, const Ogre::StringVector& params, const ButtonList& buttons );
    void                BindGameEvent( const Ogre::String& event, const ButtonList& buttons );
    void                ActivateBinds( const int button );
    void                AddGameEvents( const int button, const EventType type );

private:
    bool                    m_ButtonState[ 256 ];
    char                    m_ButtonText[ 256 ];

    bool                    m_RepeatFirstWait;
    float                   m_RepeatTimer;

    InputEventArray         m_EventQueue;

    // binds
    struct BindInfo
    {
        BindInfo():
            cmd( NULL )
        {};

        ConfigCmd* cmd;
        Ogre::StringVector params;
        ButtonList buttons;
    };
    std::vector< BindInfo > m_Binds;
    struct BindGameEventInfo
    {
        Ogre::String event;
        ButtonList buttons;
    };
    std::vector< BindGameEventInfo > m_BindGameEvents;
};



#endif // INPUT_FILTER_H
