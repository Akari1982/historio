#include "EntityMovable.h"



EntityMovable::EntityMovable( Ogre::SceneNode* node ):
    Entity( node )
{
}



EntityMovable::~EntityMovable()
{
}



void
EntityMovable::Update()
{
}



void
EntityMovable::UpdateDebug()
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



void
EntityMovable::SetPosition( const Ogre::Vector3& pos )
{
    if( m_MovePath.back() == pos )
    {
        m_MovePath.pop_back();
    }

    Entity::SetPosition( pos );
}



const Ogre::Vector3&
EntityMovable::GetMovePosition() const
{
    if( m_MovePath.size() != 0 )
    {
        return m_MovePath.back();
    }

    return GetPosition();
}
