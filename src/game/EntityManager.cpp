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

ConfigVar cv_debug_move( "debug_move", "Draw movement debug", "false" );
ConfigVar cv_debug_collision( "debug_collision", "Draw collision", "false" );

std::vector< Ogre::Vector3 > place_finder_ignore;



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
    float speed = 2.0f;

    for( size_t i = 0; i < m_EntitiesMovable.size(); ++i )
    {
        Ogre::Vector3 next = m_EntitiesMovable[ i ]->GetMoveNext();

        // if next move point exist
        if( next.z != -1 )
        {
            Ogre::Vector3 cur = m_EntitiesMovable[ i ]->GetPosition();
            Ogre::Vector3 next_cur = next - cur;
            float length = next_cur.length();
            next_cur.normalise();

            // reach end of path segment
            if( length <= ( next_cur * speed * delta ).length() )
            {
                m_EntitiesMovable[ i ]->SetPosition( next );
                cur = next;

                // remove finished segment
                std::vector< Ogre::Vector3 > move_path = m_EntitiesMovable[ i ]->GetMovePath();
                move_path.pop_back();
                m_EntitiesMovable[ i ]->SetMovePath( move_path );

                Ogre::Vector3 end = m_EntitiesMovable[ i ]->GetMoveEnd();

                //LOG_ERROR( "Update for entity " + Ogre::StringConverter::toString( i ) + ": cur_pos=" + Ogre::StringConverter::toString( cur ) + ", final_pos=" + Ogre::StringConverter::toString( end ) );

                // if we still has move segments to move
                if( move_path.size() != 0 )
                {
                    place_finder_ignore.clear();
                    m_EntitiesMovable[ i ]->SetMovePath( AStarFinder( cur, end, m_EntitiesMovable[ i ] ) );

                    //LOG_ERROR( "    path for entity " + Ogre::StringConverter::toString( m_EntitiesMovable[ i ] ) + ":" );
                    //std::vector< Ogre::Vector3 > path = m_EntitiesMovable[ i ]->GetMovePath();
                    //for( size_t j = 0; j < path.size(); ++j )
                    //{
                        //LOG_ERROR( "        " + Ogre::StringConverter::toString( path[ j ] ) );
                    //}

                    next = m_EntitiesMovable[ i ]->GetMoveNext();

                    std::vector< Ogre::Vector3 > occupation;
                    occupation.push_back( cur );
                    occupation.push_back( next );
                    //LOG_ERROR( "    occupation " + Ogre::StringConverter::toString( cur ) );
                    //LOG_ERROR( "    occupation " + Ogre::StringConverter::toString( next ) );
                    m_EntitiesMovable[ i ]->SetOccupation( occupation );
                }
                else
                {
                    //LOG_ERROR( "    occupation " + Ogre::StringConverter::toString( next ) );
                    std::vector< Ogre::Vector3 > occupation;
                    occupation.push_back( next );
                    m_EntitiesMovable[ i ]->SetOccupation( occupation );
                }
            }
            else
            {
                m_EntitiesMovable[ i ]->SetPosition( cur + next_cur * speed * delta );
            }
        }
    }

    m_Hud->Update();

    UpdateDebug();
}



void
EntityManager::UpdateDebug()
{
    if( cv_debug_collision.GetB() == true )
    {
        for( size_t i = 0; i < m_Entities.size(); ++i )
        {
            std::vector< Ogre::Vector3 > occupation = m_Entities[ i ]->GetOccupation();
            for( size_t j = 0; j < occupation.size(); ++j )
            {
                Ogre::Vector3 pos_s = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( occupation[ j ].x - 0.5f, occupation[ j ].y - 0.5f, 0 ) );
                Ogre::Vector3 pos_e = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( occupation[ j ].x + 0.5f, occupation[ j ].y + 0.5f, 0 ) );
                DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 1, 1, 0.5f ) );
                DEBUG_DRAW.Quad( pos_s.x, pos_s.y, pos_e.x, pos_s.y, pos_e.x, pos_e.y, pos_s.x, pos_e.y );
            }
        }
    }

    if( cv_debug_move.GetB() == true )
    {
        for( size_t i = 0; i < m_EntitiesMovable.size(); ++i )
        {
            Ogre::Vector3 pos = m_EntitiesMovable[ i ]->GetPosition();
            std::vector< Ogre::Vector3 > path = m_EntitiesMovable[ i ]->GetMovePath();
            for( size_t i = 0; i < path.size(); ++i )
            {
                Ogre::Vector3 pos_s;
                Ogre::Vector3 pos_e;
                if( i == 0 )
                {
                    pos_s = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( pos.x, pos.y, 0 ) );
                    pos_e = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( path.back().x, path.back().y, 0 ) );
                }
                else
                {
                    pos_s = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( path[ i - 1 ].x, path[ i - 1 ].y, 0 ) );
                    pos_e = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( path[ i ].x, path[ i ].y, 0 ) );
                }
                DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 1, 1, 1 ) );
                DEBUG_DRAW.Line( pos_s.x, pos_s.y, pos_e.x, pos_e.y );
            }
        }
    }

    for( size_t i = 0; i < m_EntitiesSelected.size(); ++i )
    {
        Ogre::Vector4 col = m_EntitiesSelected[ i ]->GetDrawBox();
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
            //Ogre::Vector3 pos_s = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( i - 0.5f, j - 0.5f, 0 ) );
            //Ogre::Vector3 pos_e = CameraManager::getSingleton().ProjectPointToScreen( Ogre::Vector3( i + 0.5f, j + 0.5f, 0 ) );
            //int pass = m_MapSector.GetPass( i, j );
            //DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 1, 1, 1 ) );
            //DEBUG_DRAW.Text( Ogre::Vector3( i, j, 0 ), 0, 0, Ogre::StringConverter::toString( pass ) );
            //DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 1, 0, 0.3f ) );
            //DEBUG_DRAW.Quad( pos_s.x, pos_s.y, pos_e.x, pos_s.y, pos_e.x, pos_e.y, pos_s.x, pos_e.y );
        }
    }

    //Ogre::SceneNode::ChildNodeIterator node = m_SceneNode->getChildIterator();
    //int row = 0;
    //DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 1, 1, 1 ) );
    //while( node.hasMoreElements() )
    //{
        //Ogre::SceneNode* n = ( Ogre::SceneNode* )node.getNext();
        //DEBUG_DRAW.Text( 10, 10 + row * 20, n->getName() );
        //++row;
    //}

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
                m_EntitiesMovable.push_back( ( EntityMovable* )entity );
                std::vector< Ogre::Vector3 > occupation;
                occupation.push_back( Ogre::Vector3( x, y, 0 ) );
                entity->SetOccupation( occupation );
            }
            else if( m_EntityDescs[ i ].entity_class == "Stand" )
            {
                entity = new EntityStand( node );
                std::vector< Ogre::Vector3 > occupation;
                for( size_t j = 0; j < m_EntityDescs[ i ].occupation.size(); ++j )
                {
                    occupation.push_back( Ogre::Vector3( x, y, 0 ) + m_EntityDescs[ i ].occupation[ j ] );
                }
                entity->SetOccupation( occupation );
            }
            else
            {
                LOG_ERROR( "EntityManager::AddEntityByName: entity class \"" + m_EntityDescs[ i ].entity_class + "\" not found." );
                return;
            }

            entity->SetPosition( Ogre::Vector3( x, y, 0 ) );
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
        Ogre::Vector4 col = m_EntitiesMovable[ i ]->GetDrawBox();

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
    //LOG_ERROR( "Start move: target=" + Ogre::StringConverter::toString( move ) );
    for( size_t i = 0; i < m_EntitiesSelected.size(); ++i )
    {
        m_EntitiesSelected[ i ]->SetMoveEnd( move );

        std::vector< Ogre::Vector3 > move_path = m_EntitiesSelected[ i ]->GetMovePath();
        Ogre::Vector3 start;
        if( move_path.size() != 0 )
        {
            start = move_path.back();
        }
        else
        {
            start = m_EntitiesSelected[ i ]->GetPosition();
        }

        place_finder_ignore.clear();
        std::vector< Ogre::Vector3 > move_path_new = AStarFinder( start, move, m_EntitiesSelected[ i ] );

        // if segment not finished add new segment to it
        if( move_path.size() != 0 )
        {
            move_path = move_path_new;
            move_path.push_back( start );
        }
        else
        {
            move_path = move_path_new;
        }
        m_EntitiesSelected[ i ]->SetMovePath( move_path );

        //LOG_ERROR( "    path for entity " + Ogre::StringConverter::toString( i ) + ":" );
        //std::vector< Ogre::Vector3 > path = m_EntitiesSelected[ i ]->GetMovePath();
        //for( size_t j = 0; j < path.size(); ++j )
        //{
            //LOG_ERROR( "        " + Ogre::StringConverter::toString( path[ j ] ) );
        //}

        std::vector< Ogre::Vector3 > occupation_old = m_EntitiesSelected[ i ]->GetOccupation();
        std::vector< Ogre::Vector3 > occupation;
        if( occupation_old.size() > 0 )
        {
            occupation.push_back( occupation_old[ 0 ] );
            //LOG_ERROR( "    occupation " + Ogre::StringConverter::toString( occupation_old[ 0 ] ) );
            Ogre::Vector3 pos = m_EntitiesSelected[ i ]->GetMoveNext();
            if( pos.z != -1 )
            {
                //LOG_ERROR( "    occupation " + Ogre::StringConverter::toString( pos ) );
                occupation.push_back( pos );
            }
            m_EntitiesSelected[ i ]->SetOccupation( occupation );
        }
    }
}



std::vector< Ogre::Vector3 >
EntityManager::AStarFinder( const Ogre::Vector3& start, const Ogre::Vector3& end, EntityMovable* self ) const
{
    std::vector< Ogre::Vector3 > move_path;

    if( self == NULL )
    {
        return move_path;
    }

    Ogre::Vector3 pos_e = PlaceFinder( end, self );
    if( pos_e.z == -1 )
    {
        return move_path;
    }

    if( start == pos_e )
    {
        return move_path;
    }

    std::vector< AStarNode* > grid;
    for( size_t i = 0; i < 100; ++i )
    {
        for( size_t j = 0; j < 100; ++j )
        {
            AStarNode* node = new AStarNode();
            node->x = i;
            node->y = j;
            node->g = 0.0f;
            node->h = 0.0f;
            node->f = 0.0f;
            node->opened = false;
            node->closed = false;
            node->parent = NULL;
            grid.push_back( node );
        }
    }

    AStarNode* start_node = grid[ start.x * 100 + start.y ];
    start_node->opened = true;

    std::vector< AStarNode* > open_list;
    open_list.push_back( start_node );

    while( open_list.size() != 0 )
    {
        AStarNode* node = open_list.back();
        open_list.pop_back();
        node->closed = true;

        //LOG_ERROR( "cycle for node: " + Ogre::StringConverter::toString( node->x ) + " " + Ogre::StringConverter::toString( node->y ) );

        // if reached the end position, construct the path and return it
        if( node->x == pos_e.x && node->y == pos_e.y )
        {
            while( node->parent != NULL )
            {
                Ogre::Vector3 point;
                point.x = node->x;
                point.y = node->y;
                point.z = 0;
                move_path.push_back( point );
                node = node->parent;
            }
            break;
        }

        std::vector< AStarNode* > neighbors;
        if( IsPassable( Ogre::Vector3( node->x - 1, node->y - 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x, node->y - 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x - 1, node->y, 0 ), self ) )
        {
            neighbors.push_back( grid[ ( node->x - 1 ) * 100 + ( node->y -  1) ] );
        }
        if( IsPassable( Ogre::Vector3( node->x, node->y - 1, 0 ), self ) )
        {
            neighbors.push_back( grid[ node->x * 100 + ( node->y - 1 ) ] );
        }
        if( IsPassable( Ogre::Vector3( node->x + 1, node->y - 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x, node->y - 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x + 1, node->y, 0 ), self ) )
        {
            neighbors.push_back( grid[ ( node->x + 1 ) * 100 + ( node->y - 1 ) ] );
        }
        if( IsPassable( Ogre::Vector3( node->x - 1, node->y, 0 ), self ) )
        {
            neighbors.push_back( grid[ ( node->x - 1 ) * 100 + node->y ] );
        }
        if( IsPassable( Ogre::Vector3( node->x + 1, node->y, 0 ), self ) )
        {
            neighbors.push_back( grid[ ( node->x + 1 ) * 100 + node->y ] );
        }
        if( IsPassable( Ogre::Vector3( node->x - 1, node->y + 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x, node->y + 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x - 1, node->y, 0 ), self ) )
        {
            neighbors.push_back( grid[ ( node->x - 1 ) * 100 + ( node->y + 1 ) ] );
        }
        if( IsPassable( Ogre::Vector3( node->x, node->y + 1, 0 ), self ) )
        {
            neighbors.push_back( grid[ node->x * 100 + ( node->y + 1 ) ] );
        }
        if( IsPassable( Ogre::Vector3( node->x + 1, node->y + 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x, node->y + 1, 0 ), self ) && IsPassable( Ogre::Vector3( node->x + 1, node->y, 0 ), self ) )
        {
            neighbors.push_back( grid[ ( node->x + 1 ) * 100 + ( node->y + 1 ) ] );
        }
        for( size_t i = 0; i < neighbors.size(); ++i )
        {
            AStarNode* neighbor = neighbors[ i ];

            if( neighbor->closed == true )
            {
                continue;
            }

            // get the distance between current node and the neighbor and calculate the next g score
            float ng = node->g + sqrt( ( neighbor->x - node->x ) * ( neighbor->x - node->x ) + ( neighbor->y - node->y ) * ( neighbor->y - node->y ) );

            // check if the neighbor has not been inspected yet, or can be reached with smaller cost from the current node
            if( neighbor->opened == false || ng < neighbor->g )
            {
                neighbor->g = ng;
                neighbor->h = sqrt( ( neighbor->x - end.x ) * ( neighbor->x - end.x ) + ( neighbor->y - end.y ) * ( neighbor->y - end.y ) );
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = node;

                if( neighbor->opened == false )
                {
                    open_list.push_back( neighbor );
                    neighbor->opened = true;
                }

                struct
                {
                    bool operator()( AStarNode* a, AStarNode* b ) const
                    {
                        return a->f > b->f;
                    }
                } less;
                std::sort( open_list.begin(), open_list.end(), less );
            }
        }
    }

    for( size_t i = 0; i < grid.size(); ++i )
    {
        delete grid[ i ];
    }

    if( move_path.size() == 0 )
    {
        place_finder_ignore.push_back( pos_e );
        move_path = AStarFinder( start, end, self );
    }

    return move_path;
}



const Ogre::Vector3
EntityManager::PlaceFinder( const Ogre::Vector3& pos, Entity* self ) const
{
    std::vector< Ogre::Vector3 > searched;
    std::vector< Ogre::Vector3 > neighbors;

    neighbors.push_back( pos );

    for( size_t i = 0; i < neighbors.size(); ++i )
    {
        Ogre::Vector3 neighbor = neighbors[ i ];

        size_t j = 0;
        for( ; j < searched.size(); ++j )
        {
            if( searched[ j ].x == neighbor.x && searched[ j ].y == neighbor.y )
            {
                break;
            }
        }
        if( j == searched.size() )
        {
            float dist = sqrt( ( pos.x - neighbor.x ) * ( pos.x - neighbor.x ) + ( pos.y - neighbor.y ) * ( pos.y - neighbor.y ) );
            if( dist <= 5.0f )
            {
                if( IsPassable( neighbor, self ) == true )
                {
                    return Ogre::Vector3( neighbor.x, neighbor.y, 0 );
                }

                neighbors.push_back( Ogre::Vector3( neighbor.x, neighbor.y - 1, 0 ) );
                neighbors.push_back( Ogre::Vector3( neighbor.x - 1, neighbor.y, 0 ) );
                neighbors.push_back( Ogre::Vector3( neighbor.x + 1, neighbor.y, 0 ) );
                neighbors.push_back( Ogre::Vector3( neighbor.x, neighbor.y + 1, 0 ) );
            }

            searched.push_back( neighbor );
        }
    }

    return Ogre::Vector3( 0, 0, -1 );
}



const bool
EntityManager::IsPassable( const Ogre::Vector3& pos, Entity* self ) const
{
    //LOG_ERROR( "IsPassable x=" + Ogre::StringConverter::toString( pos.x ) + ", y=" + Ogre::StringConverter::toString( pos.y ) );

    // we use self position collision to not to move to same point as we already stand
    if( self != NULL )
    {
        Ogre::Vector3 self_pos = self->GetPosition();
        if( self_pos == pos )
        {
            //LOG_ERROR( "    entity pos - return false" );
            return false;
        }
    }

    for( size_t i = 0; i < place_finder_ignore.size(); ++i )
    {
        if( place_finder_ignore[ i ] == pos )
        {
            //LOG_ERROR( "    ignored place - return false" );
            return false;
        }
    }

    if( m_MapSector.GetPass( pos.x, pos.y ) == 0 )
    {
        for( size_t i = 0; i < m_Entities.size(); ++i )
        {
            if( m_Entities[ i ] != self )
            {
                std::vector< Ogre::Vector3 > occupation = m_Entities[ i ]->GetOccupation();
                for( size_t j = 0; j < occupation.size(); ++j )
                {
                    if( occupation[ j ] == pos )
                    {
                        //LOG_ERROR( "    text occupation " + Ogre::StringConverter::toString( i ) + ": "  + Ogre::StringConverter::toString( occupation[ j ] ) );
                        return false;
                    }
                }
            }
        }
        //LOG_ERROR( "    return true" );
        return true;
    }
    //LOG_ERROR( "    not passable on map - return false" );
    return false;
}
