#include <OgreRoot.h>
#include "../core/CameraManager.h"
#include "../core/Logger.h"
#include "../core/Timer.h"
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

    EntityXmlFile* desc_file = new EntityXmlFile( "data/entity.xml" );
    desc_file->LoadDesc();
    delete desc_file;

    MapXmlFile* map_loader = new MapXmlFile( "data/map/test.xml" );
    map_loader->LoadMap( m_MapSector );
    map_loader->LoadEntities();
    delete map_loader;

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
    float delta = Timer::getSingleton().GetGameTimeDelta();

    for( size_t i = 0; i < m_Entities.size(); ++i )
    {
        m_Entities[ i ]->Update();

        Ogre::Vector2 start = m_Entities[ i ]->GetPosition();
        Ogre::Vector2 end = m_Entities[ i ]->GetMovePosition();

        float speed = 2.0f;

        if( start != end )
        {
            Ogre::Vector2 end_start = end - start;
            float length = end_start.length();
            end_start.normalise();

            if( length < (end_start * speed * delta).length() )
            {
                m_Entities[ i ]->SetPosition( end );
            }
            else
            {
                m_Entities[ i ]->SetPosition( start + end_start * speed * delta );
            }
        }
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
EntityManager::AddEntityByName( const Ogre::String& name, const float x, const float y )
{
    for( size_t i = 0; i < m_EntityDescs.size(); ++i )
    {
        if( m_EntityDescs[ i ].name == name )
        {
            Entity* entity = new Entity();
            entity->SetSize( Ogre::Vector2( m_EntityDescs[ i ].width, m_EntityDescs[ i ].height ) );
            entity->SetPosition( Ogre::Vector2( x, y ) );
            entity->SetMovePosition( Ogre::Vector2( x, y ) );
            entity->SetTexture( m_EntityDescs[ i ].texture );
            entity->UpdateGeometry();
            m_Entities.push_back( entity );
            return;
        }
    }
    LOG_ERROR( "EntityManager::AddEntityByName: entity \"" + name + "\" not found in desc." );
}



void
EntityManager::AddEntityDesc( const EntityDesc& desc )
{
    for( size_t i = 0; i < m_EntityDescs.size(); ++i )
    {
        if( m_EntityDescs[ i ].name == desc.name )
        {
            m_EntityDescs[ i ] = desc;
            return;
        }
    }
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

        m_MapSector.Render();

        for( size_t i = 0; i < m_Entities.size(); ++i )
        {
            m_Entities[ i ]->Render();
        }
    }
}
