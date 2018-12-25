#include "EntityMovable.h"



EntityMovable::EntityMovable( Ogre::SceneNode* node ):
    Entity( node )
{
}



EntityMovable::~EntityMovable()
{
}



void
EntityMovable::SetMovePath( std::vector< Ogre::Vector3 >& move_path )
{
    m_MovePath = move_path;
}



const std::vector< Ogre::Vector3 >&
EntityMovable::GetMovePath() const
{
    return m_MovePath;
}



Ogre::Vector3
EntityMovable::GetMoveNext() const
{
    if( m_MovePath.size() != 0 )
    {
        return m_MovePath.back();
    }
    return Ogre::Vector3( 0, 0, -1 );
}



void
EntityMovable::SetMoveEnd( const Ogre::Vector3& end )
{
    m_MoveEnd = end;
}



const Ogre::Vector3&
EntityMovable::GetMoveEnd() const
{
    return m_MoveEnd;
}
