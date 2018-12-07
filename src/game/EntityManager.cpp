#include <OgreRoot.h>
#include "../core/CameraManager.h"
#include "../core/Logger.h"
#include "Entity.h"
#include "EntityManager.h"
#include "EntityManagerCommands.h"
#include "EntityXmlFile.h"
#include "MapXmlFile.h"



template<>EntityManager *Ogre::Singleton< EntityManager >::msSingleton = NULL;



EntityManager::EntityManager()
{
    LOG_TRIVIAL( "EntityManager created." );

    InitCmd();

    EntityXmlFile* desc_file = new EntityXmlFile( "data/entity.xml" )
    desc_file.LoadDesc();
    delete desc_file;

    MapXmlFile* map_loader = new MapXmlFile( "data/map/test.xml" );
    map_loader->LoadMap( m_MapSector );
    delete map_loader;

    Entity* entity = new Entity();
    m_Entities.push_back( entity );

    m_SceneManager = Ogre::Root::getSingletonPtr()->getSceneManager( "Scene" );
    m_RenderSystem = Ogre::Root::getSingleton().getRenderSystem();
    m_SceneManager->addRenderQueueListener( this );
}



EntityManager::~EntityManager()
{
    for( unsigned int i = 0; i < m_Entities.size(); ++i )
    {
        delete m_Entities[ i ];
    }

    m_SceneManager->removeRenderQueueListener( this );

    LOG_TRIVIAL( "EntityManager destroyed." );
}



void
EntityManager::Input( const Event& event )
{
}



void
EntityManager::Update()
{
    for( size_t i = 0; i < m_Entities.size(); ++i )
    {
        m_Entities[ i ]->Update();
    }

    UpdateDebug();
}



void
EntityManager::UpdateDebug()
{
    for( size_t i = 0; i < m_Entities.size(); ++i )
    {
        m_Entities[ i ]->UpdateDebug();
    }
}



void
EntityManager::OnResize()
{
}



void
EntityManager::AddEntityDesc( EntityDesc desc )
{
    m_EntityDescs.push_back( desc );
}



void
EntityManager::renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation )
{
    if( queueGroupId == Ogre::RENDER_QUEUE_MAIN )
    {
        m_RenderSystem->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
        m_RenderSystem->_setViewMatrix( CameraManager::getSingleton().GetCurrentCamera()->getViewMatrix( true ) );
        m_RenderSystem->_setProjectionMatrix( CameraManager::getSingleton().GetCurrentCamera()->getProjectionMatrixRS() );
        for( size_t i = 0; i < m_Entities.size(); ++i )
        {
            m_Entities[ i ]->Render();
        }
    }
}
