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
    Ogre::Camera* m_Camera;
    Ogre::Viewport* m_Viewport;

    bool m_Selection;
    Ogre::Vector3 m_SelectionStart;
    Ogre::Vector3 m_SelectionEnd;
};



#endif // HUD_MANAGER_H
