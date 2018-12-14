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

    EntityXmlFile* desc_file = new EntityXmlFile( "data/entities.xml" );
    desc_file->LoadDesc();
    delete desc_file;

    m_SceneManager = Ogre::Root::getSingletonPtr()->getSceneManager( "Scene" );
    m_SceneNode = m_SceneManager->getRootSceneNode()->createChildSceneNode( "EntityManager" );

    MapXmlFile* map_loader = new MapXmlFile( "data/map/test.xml" );
    map_loader->LoadMap( m_MapSector );
    map_loader->LoadEntities();
    delete map_loader;

    Ogre::SceneNode* node = m_SceneNode->createChildSceneNode( "Map" );
    node->attachObject( &m_MapSector );
}



EntityManager::~EntityManager()
{
    for( unsigned int i = 0; i < m_Entities.size(); ++i )
    {
        delete m_Entities[ i ];
    }

    m_SceneManager->getRootSceneNode()->removeAndDestroyChild( "EntityManager" );

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

        DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 1, 1, 1 ) );
        DEBUG_DRAW.Line( pos_s.x, pos_s.y, pos_e.x, pos_e.y );
    }

    for( size_t i = 0; i < m_EntitiesSelected.size(); ++i )
    {
        Ogre::Vector4 col = m_EntitiesSelected[ i ]->GetCollisionBox();
        Ogre::Vector3 pos = m_EntitiesSelected[ i ]->GetPosition();
        Ogre::Vector3 pos_s = CameraManager::getSingleton().ProjectPointToScreen( pos + Ogre::Vector3( col.x, col.y, 0 ) );
        Ogre::Vector3 pos_e = CameraManager::getSingleton().ProjectPointToScreen( pos + Ogre::Vector3( col.z, col.w, 0 ) );

        DEBUG_DRAW.SetColour( Ogre::ColourValue( 0.5f, 1, 0, 0.3f ) );
        DEBUG_DRAW.Quad( pos_s.x, pos_s.y, pos_e.x, pos_s.y, pos_e.x, pos_e.y, pos_s.x, pos_e.y );
    }

    for( size_t i = 0; i < 100; ++i )
    {
        for( size_t j = 0; j < 100; ++j )
        {
            Ogre::Vector3 pos_s = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( i - 0.5f, j - 0.5f, 0 ) );
            Ogre::Vector3 pos_e = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( i + 0.5f, j + 0.5f, 0 ) );

            int pass = m_MapSector.GetPass( i, j );
            DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 0, 0, 0.3f ) );
            DEBUG_DRAW.Text( Ogre::Vector3( i, j, 0 ), 0, 0, Ogre::StringConverter::toString( pass ) );
            DEBUG_DRAW.Quad( pos_s.x, pos_s.y, pos_e.x, pos_s.y, pos_e.x, pos_e.y, pos_s.x, pos_e.y );
        }
    }

    Ogre::SceneNode::ChildNodeIterator node = m_SceneNode->getChildIterator();
    int row = 0;
    DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 1, 1, 1 ) );
    while( node.hasMoreElements() )
    {
        Ogre::SceneNode* n = ( Ogre::SceneNode* )node.getNext();
        DEBUG_DRAW.Text( 10, 10 + row * 20, n->getName() );
        ++row;
    }

    m_Hud->UpdateDebug();
}



void
EntityManager::AddEntityByName( const Ogre::String& name, const float x, const float y )
{
    static int UNIQUE_NODE_ID = 0;

    for( size_t i = 0; i < m_EntityDescs.size(); ++i )
    {
        if( m_EntityDescs[ i ].name == name )
        {
            Entity* entity;
            Ogre::SceneNode* node = m_SceneNode->createChildSceneNode( "Entity" + m_EntityDescs[ i ].entity_class + Ogre::StringConverter::toString( UNIQUE_NODE_ID ) );
            ++UNIQUE_NODE_ID;


            if( m_EntityDescs[ i ].entity_class == "Movable" )
            {
                entity = new EntityMovable( node );
                (( EntityMovable* )entity)->SetMovePosition( Ogre::Vector3( x, y, 0 ) );
                m_EntitiesMovable.push_back( ( EntityMovable* )entity );
            }
            else if( m_EntityDescs[ i ].entity_class == "Stand" )
            {
                entity = new EntityStand( node );
            }
            else
            {
                LOG_ERROR( "EntityManager::AddEntityByName: entity class \"" + m_EntityDescs[ i ].entity_class + "\" not found." );
                return;
            }

            entity->SetPosition( Ogre::Vector3( x, y, 0 ) );
            entity->SetCollisionBox( m_EntityDescs[ i ].collision_box );
            entity->SetDrawBox( m_EntityDescs[ i ].draw_box );
            entity->SetTexture( m_EntityDescs[ i ].texture );
            entity->UpdateGeometry();
            m_Entities.push_back( entity );

            node->attachObject( entity );

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

        if( rhs_left < lhs_right && rhs_right > lhs_left && rhs_top < lhs_bottom && rhs_bottom > lhs_top )
        {
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
}
