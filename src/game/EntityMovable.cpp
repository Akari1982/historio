#include "EntityMovable.h"



EntityMovable::EntityMovable( Ogre::SceneNode* node ):
    Entity( node ),
    m_MovePosition( Ogre::Vector3::ZERO )
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
SetMovePath( std::vector< Ogre::Vector3 >& move_path )
{
    m_MovePath = move_path;
}



const std::vector< Ogre::Vector3 >&
GetMovePath() const
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
