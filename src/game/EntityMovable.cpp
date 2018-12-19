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
    if( m_MovePath.size() != 0 )
    {
        Ogre::Vector3 pos = m_MovePath.back();
        m_MovePath = move_path;
        m_MovePath.push_back( pos );
    }
    else
    {
        m_MovePath = move_path;
    }
}



const std::vector< Ogre::Vector3 >&
EntityMovable::GetMovePath() const
{
    return m_MovePath;
}



void
EntityMovable::SetPosition( const Ogre::Vector3& pos )
{
    if( m_MovePath.size() != 0 )
    {
        if( m_MovePath.back() == pos )
        {
            m_MovePath.pop_back();
        }
    }

    Entity::SetPosition( pos );
}



Ogre::Vector3
EntityMovable::GetMoveNextPosition() const
{
    if( m_MovePath.size() != 0 )
    {
        return m_MovePath.back();
    }
    return GetPosition();
}



Ogre::Vector3
EntityMovable::GetMoveEndPosition() const
{
    if( m_MovePath.size() != 0 )
    {
        return m_MovePath.front();
    }
    return GetPosition();
}
