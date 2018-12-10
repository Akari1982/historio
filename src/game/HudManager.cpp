#include <OgreRoot.h>
#include "../core/Logger.h"
#include "../core/Timer.h"
#include "HudManager.h"



template<>HudManager *Ogre::Singleton< HudManager >::msSingleton = NULL;



HudManager::HudManager()
{
    LOG_TRIVIAL( "HudManager created." );
}



HudManager::~HudManager()
{
    LOG_TRIVIAL( "HudManager destroyed." );
}



void
HudManager::Input( const Event& event )
{
    if( event.type == ET_PRESS && event.param1 == OIS::MB_Right )
    {
        m_Selection = true;
        m_SelectionStart.x = event.param3;
        m_SelectionStart.y = event.param4;
    }
    else if( event.type == ET_RELEASE && event.param1 == OIS::MB_Right )
    {
        m_Selection = false;
    }
    else if( event.type == ET_MOUSE_MOVE && m_Selection == true )
    {
        m_SelectionSize.x = m_SelectionStart.x - event.param3;
        m_SelectionSize.y = m_SelectionStart.y - event.param4; 
    }

    LOG_ERROR( "HUD: " + Ogre::StringConverter::toString( event.param3 ) + " " + Ogre::StringConverter::toString( event.param4 ) );
}



void
HudManager::Update()
{
    float delta = Timer::getSingleton().GetGameTimeDelta();
}



void
HudManager::UpdateDebug()
{
}
