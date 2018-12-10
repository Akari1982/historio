#include "EntityMovable.h"



EntityMovable::EntityMovable():
    m_MovePosition( Ogre::Vector2( 0, 0 ) )
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
EntityMovable::SetMovePosition( const Ogre::Vector2& target )
{
    m_MovePosition = target;
}



const Ogre::Vector2&
EntityMovable::GetMovePosition() const
{
    return m_MovePosition;
}
