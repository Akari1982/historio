#include <OgreRoot.h>
#include "../core/CameraManager.h"
#include "../core/DebugDraw.h"
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
    if( event.type == ET_PRESS && event.button == OIS::MB_Left )
    {
        m_Selection = true;
        Ogre::Camera* camera = CameraManager::getSingleton().GetCurrentCamera();

        Ogre::Ray ray = camera->getCameraToViewportRay( event.param3 / Ogre::Root::getSingleton().getRenderTarget( "QGearsWindow" )->getViewport( 0 )->getActualWidth(), event.param4 / Ogre::Root::getSingleton().getRenderTarget( "QGearsWindow" )->getViewport( 0 )->getActualHeight() );
        Ogre::Plane plane( Ogre::Vector3::UNIT_Z, 0 );
        std::pair< bool, Ogre::Real > res = ray.intersects( plane );
        Ogre::Vector3 point = ray.getPoint( res.second );

        LOG_ERROR( "HUD: " + Ogre::StringConverter::toString( point ) + " " + Ogre::StringConverter::toString( ray.getDirection() ) );
        m_SelectionStart.x = event.param3;
        m_SelectionStart.y = event.param4;
    }
    else if( event.type == ET_RELEASE && event.button == OIS::MB_Left )
    {
        m_Selection = false;
        m_SelectionSize.x = 0;
        m_SelectionSize.y = 0;
    }
    else if( event.type == ET_MOUSE_MOVE && m_Selection == true )
    {
        m_SelectionSize.x = event.param3 - m_SelectionStart.x;
        m_SelectionSize.y = event.param4 - m_SelectionStart.y; 
    }
}



void
HudManager::Update()
{
}



void
HudManager::UpdateDebug()
{
    if( m_Selection == true )
    {
        DEBUG_DRAW.SetColour( Ogre::ColourValue( 0, 1, 0, 0.3f) );
        DEBUG_DRAW.Quad( m_SelectionStart.x, m_SelectionStart.y, m_SelectionStart.x + m_SelectionSize.x, m_SelectionStart.y, m_SelectionStart.x + m_SelectionSize.x, m_SelectionStart.y + m_SelectionSize.y, m_SelectionStart.x, m_SelectionStart.y + m_SelectionSize.y );
    }
}
