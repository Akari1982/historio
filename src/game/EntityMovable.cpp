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

    if( m_MovePath.size() != 0 )
    {
        m_Occupation.push_back( m_MovePath.back() );
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

            m_Occupation.clear();
            m_Occupation.push_back( pos );
        }
    }

    Entity::SetPosition( pos );
}



const Ogre::Vector3&
EntityMovable::GetMoveNextPosition() const
{
    if( m_MovePath.size() != 0 )
    {
        return m_MovePath.back();
    }
    return GetPosition();
}



const Ogre::Vector3&
EntityMovable::GetMoveEndPosition() const
{
    if( m_MovePath.size() != 0 )
    {
        return m_MovePath.first();
    }
    return GetPosition();
}
