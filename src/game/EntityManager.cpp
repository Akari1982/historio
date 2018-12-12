#include <OgreRoot.h>
#include "../core/CameraManager.h"
#include "../core/DebugDraw.h"
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

    m_Hud = new HudManager();

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

    delete m_Hud;

    LOG_TRIVIAL( "EntityManager destroyed." );
}



void
EntityManager::Input( const Event& event )
{
    m_Hud->Input( event );
}



void
EntityManager::Update()
{
    float delta = Timer::getSingleton().GetGameTimeDelta();

    for( size_t i = 0; i < m_Entities.size(); ++i )
    {
        m_Entities[ i ]->Update();
    }

    for( size_t i = 0; i < m_EntitiesMovable.size(); ++i )
    {
        Ogre::Vector3 start = m_EntitiesMovable[ i ]->GetPosition();
        Ogre::Vector3 end = m_EntitiesMovable[ i ]->GetMovePosition();

        float speed = 2.0f;

        if( start != end )
        {
            Ogre::Vector3 end_start = end - start;
            float length = end_start.length();
            end_start.normalise();

            if( length < (end_start * speed * delta).length() )
            {
                m_EntitiesMovable[ i ]->SetPosition( end );
            }
            else
            {
                m_EntitiesMovable[ i ]->SetPosition( start + end_start * speed * delta );
            }
        }
    }

    m_Hud->Update();

    UpdateDebug();
}



void
EntityManager::UpdateDebug()
{
    for( size_t i = 0; i < m_Entities.size(); ++i )
    {
        m_Entities[ i ]->UpdateDebug();
    }

    for( size_t i = 0; i < m_EntitiesMovable.size(); ++i )
    {
        Ogre::Vector3 pos_s = CameraManager::getSingleton().ProjectPointToScreen( m_EntitiesMovable[ i ]->GetPosition() );
        Ogre::Vector3 pos_e = CameraManager::getSingleton().ProjectPointToScreen( m_EntitiesMovable[ i ]->GetMovePosition() );

        DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 0, 0, 1 ) );
        DEBUG_DRAW.Line( pos_s.x, pos_s.y, pos_e.x, pos_e.y );
    }

    for( size_t i = 0; i < m_EntitiesSelected.size(); ++i )
    {
        Ogre::Vector4 col = m_EntitiesMovable[ i ]->GetCollisionBox();

        DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 0, 0, 0.3f ) );
        DEBUG_DRAW.Quad( col.x, col.y, col.z, col.y, col.z, col.w, col.x, col.w );
    }

    m_Hud->UpdateDebug();
}



void
EntityManager::AddEntityByName( const Ogre::String& name, const float x, const float y )
{
    for( size_t i = 0; i < m_EntityDescs.size(); ++i )
    {
        if( m_EntityDescs[ i ].name == name )
        {
            Entity* entity;

            if( m_EntityDescs[ i ].entity_class == "Movable" )
            {
                entity = new EntityMovable();
                (( EntityMovable* )entity)->SetMovePosition( Ogre::Vector3( x + 10, y, 0 ) );
                m_EntitiesMovable.push_back( ( EntityMovable* )entity );
            }
            else if( m_EntityDescs[ i ].entity_class == "Stand" )
            {
                entity = new EntityStand();
            }
            else
            {
                LOG_ERROR( "EntityManager::AddEntityByName: entity class \"" + m_EntityDescs[ i ].entity_class + "\" not found." );
                return;
            }

            entity->SetPosition( Ogre::Vector3( x, y, 0 ) );
            entity->SetCollisionBox( m_EntityDescs[ i ].collision_box );
            entity->SetDrawBox( m_EntityDescs[ i ].draw_box );
            entity->SetSize( Ogre::Vector2( m_EntityDescs[ i ].width, m_EntityDescs[ i ].height ) );
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
EntityManager::SetEntitySelection( const Ogre::Vector3& start, const Ogre::Vector3& end )
{
    //LOG_ERROR( "SetEntitySelection: " + Ogre::StringConverter::toString( start ) + " " + Ogre::StringConverter::toString( end ) );

    m_EntitiesSelected.clear();

    for( size_t i = 0; i < m_EntitiesMovable.size(); ++i )
    {
        Ogre::Vector3 pos = m_EntitiesMovable[ i ]->GetPosition();
        Ogre::Vector4 col = m_EntitiesMovable[ i ]->GetCollisionBox();

        float lhs_left = ( start.x < end.x ) ? start.x : end.x;
        float lhs_right = ( start.x < end.x ) ? end.x : start.x;
        float lhs_top = ( start.y < end.y ) ? start.y : end.y;
        float lhs_bottom = ( start.y < end.y ) ? end.y : start.y;

        float rhs_left = pos.x + col.x;
        float rhs_right = pos.x + col.z;
        float rhs_top = pos.y + col.y;
        float rhs_bottom = pos.y + col.w;

        if( rhs_left > lhs_right || rhs_right < lhs_left || rhs_top > lhs_bottom || rhs_bottom < lhs_top )
        {
            //LOG_ERROR( "Found: " + Ogre::StringConverter::toString( pos ) );
            m_EntitiesSelected.push_back( m_EntitiesMovable[ i ] );
        }
    }
}



void
EntityManager::SetEntitySelectionMove( const Ogre::Vector3& move )
{
    for( size_t i = 0; i < m_EntitiesSelected.size(); ++i )
    {
        m_EntitiesSelected[ i ]->SetMovePosition( move );
    }
    m_EntitiesSelected.clear();
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
