#include "EntityManager.h"
#include "EntityManagerCommands.h"

#include <OgreEntity.h>
#include <OgreRoot.h>

#include "CameraManager.h"
#include "ConfigVar.h"
#include "DebugDraw.h"
#include "InputManager.h"
#include "Logger.h"
#include "Math.h"
#include "ScriptManager.h"
#include "Timer.h"



template<>EntityManager *Ogre::Singleton< EntityManager >::msSingleton = NULL;



EntityManager::EntityManager():
    m_MoveMask( 0x00 ),
    m_Sprite( NULL )
{
    InitCmd();

    LOG_TRIVIAL( "EntityManager created." );

    m_SceneNode = Ogre::Root::getSingleton().getSceneManager( "Scene" )->getRootSceneNode()->createChildSceneNode( "EntityManager" );

    m_SceneNode->createChildSceneNode( "Sprite" );

    Walkmesh* walkmesh = new Walkmesh();
    EntityManager::getSingleton().AddWalkmesh( walkmesh );
}



EntityManager::~EntityManager()
{
    if( m_Sprite != NULL )
    {
        delete m_Sprite;
    }

    Clear();

    Ogre::Root::getSingleton().getSceneManager( "Scene" )->getRootSceneNode()->removeAndDestroyChild( "EntityManager" );

    LOG_TRIVIAL( "EntityManager destroyed." );
}



void
EntityManager::Input( const Event& event )
{
    if( event.type == ET_REPEAT )
    {
        if( event.event == "walk_left" )
        {
            m_MoveMask |= 0x08;
        }
        else if( event.event == "walk_right" )
        {
            m_MoveMask |= 0x02;
        }
        else if( event.event == "walk_down" )
        {
            m_MoveMask |= 0x04;
        }
        else if( event.event == "walk_up" )
        {
            m_MoveMask |= 0x01;
        }
    }
}



bool
EntityManager::InputToRotation( Ogre::Degree& rotation )
{
    switch( m_MoveMask )
    {
        case 0x00: return false;
        case 0x01: rotation = Ogre::Degree( 0 ); return true;
        case 0x02: rotation = Ogre::Degree( 90 ); return true;
        case 0x03: rotation = Ogre::Degree( 45 ); return true;
        case 0x04: rotation = Ogre::Degree( 180 ); return true;
        case 0x05: rotation = Ogre::Degree( 0 ); return false;
        case 0x06: rotation = Ogre::Degree( 135 ); return true;
        case 0x07: rotation = Ogre::Degree( 90 ); return true;
        case 0x08: rotation = Ogre::Degree( 270 ); return true;
        case 0x09: rotation = Ogre::Degree( 315 ); return true;
        case 0x0a: rotation = Ogre::Degree( 0 ); return false;
        case 0x0b: rotation = Ogre::Degree( 0 ); return true;
        case 0x0c: rotation = Ogre::Degree( 225 ); return true;
        case 0x0d: rotation = Ogre::Degree( 270 ); return true;
        case 0x0e: rotation = Ogre::Degree( 180 ); return true;
        case 0x0f: rotation = Ogre::Degree( 0 ); return false;
    }
    return false;
}



void
EntityManager::Update()
{
    if( m_Sprite != NULL )
    {
        m_Sprite->Update();
    }



    // update all entity scripts
    ScriptManager::getSingleton().Update( ScriptManager::ENTITY );



    UpdateMove();



    UpdateDebug();



    // reset currently active move events
    m_MoveMask = 0x00;
}



void
EntityManager::UpdateMove()
{
    // check auto movement
    for( size_t i = 0; i < m_Entity.size(); ++i )
    {
        if( m_Entity[ i ]->IsMove() == true )
        {
            EntityCalculateMove( m_Entity[ i ] );
        }
    }



    // check player move



    // perform move
}



void
EntityManager::UpdateDebug()
{
    for( size_t i = 0; i < m_Entity.size(); ++i )
    {
        m_Entity[ i ]->UpdateDebug();
    }

    for( size_t i = 0; i < m_Trigger.size(); ++i )
    {
        m_Trigger[ i ]->UpdateDebug();
    }

    for( size_t i = 0; i < m_SpawnPoint.size(); ++i )
    {
        m_SpawnPoint[ i ]->UpdateDebug();
    }
}



void
EntityManager::OnResize()
{
}



void
EntityManager::Clear()
{
    for( size_t i = 0; i < m_Walkmesh.size(); ++i )
    {
        delete m_Walkmesh[ i ];
    }
    m_Walkmesh.clear();

    for( size_t i = 0; i < m_SpawnPoint.size(); ++i )
    {
        delete m_SpawnPoint[ i ];
    }
    m_SpawnPoint.clear();

    for( unsigned int i = 0; i < m_Entity.size(); ++i )
    {
        delete m_Entity[ i ];
    }
    m_Entity.clear();


    for( unsigned int i = 0; i < m_Trigger.size(); ++i )
    {
        delete m_Trigger[ i ];
    }
    m_Trigger.clear();

    m_SceneNode->removeAndDestroyAllChildren();
}



void
EntityManager::AddWalkmesh( Walkmesh* walkmesh )
{
    m_Walkmesh.push_back( walkmesh );
}



void
EntityManager::AddSpawnPoint( SpawnPoint* spawn_point )
{
    m_SpawnPoint.push_back( spawn_point );
}



void
EntityManager::AddEntity( const Ogre::String& name, const Ogre::String& file_name, const Ogre::Vector3& position, const Ogre::Degree& rotation, const bool script )
{
    Ogre::SceneNode* node = m_SceneNode->createChildSceneNode( "Model_" + name );
    Entity* entity = new Entity( name, node );

    if( file_name != "" )
    {
        entity->SetModel( file_name );
    }

    if( script == true )
    {
        ScriptManager::getSingleton().AddEntity( ScriptManager::ENTITY, name, entity );
    }

    entity->SetPosition( position );
    entity->SetRotation( rotation );
    m_Entity.push_back( entity );
}



void
EntityManager::AddSprite( const Ogre::String& name )
{
    if( m_Sprite != NULL )
    {
        delete m_Sprite;
    }

    m_Sprite = new Sprite( name, ( Ogre::SceneNode* )m_SceneNode->getChild( "Sprite" ) );
}



void
EntityManager::AddSquareTrigger( const Ogre::String& name, const Ogre::Vector3& point1, const Ogre::Vector3& point2, const Ogre::Vector3& point3, const Ogre::Vector3& point4, const bool enabled )
{
    Trigger* trigger = new Trigger( name );
    trigger->SetSquareTrigger( point1, point2, point3, point4 );
    m_Trigger.push_back( trigger );
}



void
EntityManager::EntityToSpawnPoint( Entity* entity, const Ogre::String& point_name )
{
    for( size_t i = 0; i < m_SpawnPoint.size(); ++i )
    {
        if( m_SpawnPoint[ i ]->GetName() == point_name )
        {
            entity->SetWalkmeshId( m_SpawnPoint[ i ]->GetWalkmeshId() );
            EntityToWalkmeshByXZ( entity, m_SpawnPoint[ i ]->GetPosition() );
            break;
        }
    }
}



void
EntityManager::EntityToWalkmeshByXZ( Entity* entity, const Ogre::Vector2& position )
{
}



void
EntityManager::EntityCalculateMove( Entity* entity )
{
    Ogre::Vector3 move_vector = Ogre::Vector3( 0, 0, -1 );

    Ogre::Quaternion q;
    q.FromAngleAxis( Ogre::Radian( entity->GetRotation() ), Ogre::Vector3::UNIT_Y );
    move_vector = q * move_vector;

    if( ( entity->GetWalkmeshId() != -1 ) && ( entity->GetTriangleId() != -1 ) )
    {
        Ogre::Vector3 final_point;

        // check first rotation
        Ogre::Vector3 add_vector = Ogre::Vector3( 0, 0, -1 );
        Ogre::Vector3 check_vector = move_vector;
        q.FromAngleAxis( Ogre::Radian( entity->GetRotation() + Ogre::Degree( 22.5f ) ), Ogre::Vector3::UNIT_Y );
        add_vector = q * add_vector;
        check_vector += add_vector;
        bool check = EntityWalkmeshCheck( entity, check_vector / 50.0f, final_point );

        if( check == true )
        {
            // check second rotation
            add_vector = Ogre::Vector3( 0, 0, -1 );
            check_vector = move_vector;
            q.FromAngleAxis( Ogre::Radian( entity->GetRotation() - Ogre::Degree( 22.5f ) ), Ogre::Vector3::UNIT_Y );
            add_vector = q * add_vector;
            check_vector += add_vector;
            check = EntityWalkmeshCheck( entity, check_vector / 50.0f, final_point );

            if( check == true )
            {
                // check straight
                add_vector = Ogre::Vector3( 0, 0, -1 );
                check_vector = move_vector;
                q.FromAngleAxis( Ogre::Radian( entity->GetRotation() ), Ogre::Vector3::UNIT_Y );
                add_vector = q * add_vector;
                check_vector += add_vector;
                check = EntityWalkmeshCheck( entity, check_vector / 50.0f, final_point );

                if( check == true )
                {
                    entity->SetPosition( entity->GetPosition() + move_vector / 50.0f );
                }
            }
        }
    }
}



bool
EntityManager::EntityWalkmeshCheck( Entity* entity, const Ogre::Vector3& move_vector, Ogre::Vector3& final_point )
{
    return false;
}
