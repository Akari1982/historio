#include <OgreRoot.h>
#include "../core/CameraManager.h"
#include "../core/DebugDraw.h"
#include "../core/Logger.h"
#include "../core/Timer.h"
#include "EntityManager.h"
#include "HudManager.h"



template<>HudManager *Ogre::Singleton< HudManager >::msSingleton = NULL;



HudManager::HudManager():
    m_Selection( false )
{
    m_Camera = CameraManager::getSingleton().GetCurrentCamera();
    m_Viewport = CameraManager::getSingleton().GetCurrentViewport();

    LOG_TRIVIAL( "HudManager created." );
}



HudManager::~HudManager()
{
    LOG_TRIVIAL( "HudManager destroyed." );
}



void
HudManager::Input( const Event& event )
{
    static float mouse_x;
    static float mouse_y;

    if( m_Selection == false )
    {
        if( event.type == ET_MOUSE_PRESS && event.button == OIS::MB_Left )
        {
            mouse_x = event.param3;
            mouse_y = event.param4;

            Ogre::Ray ray = m_Camera->getCameraToViewportRay( mouse_x / m_Viewport->getActualWidth(), mouse_y / m_Viewport->getActualHeight() );
            Ogre::Plane plane( Ogre::Vector3::UNIT_Z, 0 );
            std::pair< bool, Ogre::Real > res = ray.intersects( plane );
            Ogre::Vector3 point = ray.getPoint( res.second );

            m_Selection = true;
            m_SelectionStart = point;
            m_SelectionEnd = point;
        }
        else if( event.type == ET_MOUSE_PRESS && event.button == OIS::MB_Right )
        {
            Ogre::Ray ray = m_Camera->getCameraToViewportRay( event.param3 / m_Viewport->getActualWidth(), event.param4 / m_Viewport->getActualHeight() );
            Ogre::Plane plane( Ogre::Vector3::UNIT_Z, 0 );
            std::pair< bool, Ogre::Real > res = ray.intersects( plane );
            Ogre::Vector3 point = ray.getPoint( res.second );

            // set coords rounded to integer
            EntityManager::getSingleton().SetEntitySelectionMove( Ogre::Vector3( std::floor( point.x + 0.5f ), std::floor( point.y + 0.5f ), 0 ) );
        }
    }
    else
    {
        if( event.type == ET_MOUSE_RELEASE && event.button == OIS::MB_Left )
        {
            EntityManager::getSingleton().SetEntitySelection( m_SelectionStart, m_SelectionEnd );
            m_Selection = false;
        }
        else if( event.type == ET_MOUSE_MOVE )
        {
            mouse_x = event.param3;
            mouse_y = event.param4;

            Ogre::Ray ray = m_Camera->getCameraToViewportRay( mouse_x / m_Viewport->getActualWidth(), mouse_y / m_Viewport->getActualHeight() );
            Ogre::Plane plane( Ogre::Vector3::UNIT_Z, 0 );
            std::pair< bool, Ogre::Real > res = ray.intersects( plane );
            Ogre::Vector3 point = ray.getPoint( res.second );

            m_SelectionEnd = point;
        }
        else if( event.type == ET_MOUSE_SCROLL || ( event.type == ET_REPEAT && ( event.button == OIS::KC_W || event.button == OIS::KC_A || event.button == OIS::KC_S || event.button == OIS::KC_D ) ) )
        {
            Ogre::Ray ray = m_Camera->getCameraToViewportRay( mouse_x / m_Viewport->getActualWidth(), mouse_y / m_Viewport->getActualHeight() );
            Ogre::Plane plane( Ogre::Vector3::UNIT_Z, 0 );
            std::pair< bool, Ogre::Real > res = ray.intersects( plane );
            Ogre::Vector3 point = ray.getPoint( res.second );

            m_SelectionEnd = point;
        }
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
        Ogre::Vector3 start = CameraManager::getSingleton().ProjectPointToScreen( m_SelectionStart );
        Ogre::Vector3 end = CameraManager::getSingleton().ProjectPointToScreen( m_SelectionEnd );

        DEBUG_DRAW.SetColour( Ogre::ColourValue( 0, 1, 0, 0.3f) );
        DEBUG_DRAW.Quad( start.x, start.y, end.x, start.y, end.x, end.y, start.x, end.y );
    }
}
