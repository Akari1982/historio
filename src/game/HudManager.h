#ifndef HUD_MANAGER_H
#define HUD_MANAGER_H

#include <OgreSingleton.h>
#include "../core/Event.h"



class HudManager : public Ogre::Singleton< HudManager >
{
public:
    HudManager();
    virtual ~HudManager();

    void Input( const Event& event );
    void Update();
    void UpdateDebug();

private:
    bool m_Selection;
    Ogre::Vector2 m_SelectionStart;
    Ogre::Vector2 m_SelectionSize;
};



#endif // HUD_MANAGER_H
