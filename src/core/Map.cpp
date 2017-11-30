#include <OgreRoot.h>

#include "CameraManager.h"
#include "Map.h"



Map::Map()
{
    for( int i = 0; i < 100; ++i )
    {
        for( int j = 0; j < 100; ++j )
        {
            MapTile tile;
            if( i > 20 )
            {
                tile.type = MapTile::GRASS;
            }
            else
            {
                tile.type = MapTile::WATER;
            }
            m_MapData[ i * 100 + j ] = tile;
        }
    }

    m_MapSector = new MapSector( this );

    m_SceneManager = Ogre::Root::getSingletonPtr()->getSceneManager( "Scene" );
    m_RenderSystem = Ogre::Root::getSingleton().getRenderSystem();
    m_SceneManager->addRenderQueueListener( this );
}



Map::~Map()
{
    delete m_MapSector;

    m_SceneManager->removeRenderQueueListener( this );
}



MapTile
Map::GetTile( const unsigned int x, const unsigned int y )
{
    return m_MapData[ y * 100 + x ];
}



void
Map::renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation )
{
    if( queueGroupId == Ogre::RENDER_QUEUE_MAIN )
    {
        m_RenderSystem->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
        m_RenderSystem->_setViewMatrix( CameraManager::getSingleton().GetCurrentCamera()->getViewMatrix( true ) );
        m_RenderSystem->_setProjectionMatrix( CameraManager::getSingleton().GetCurrentCamera()->getProjectionMatrixRS() );
        m_MapSector->Render();
    }
}
