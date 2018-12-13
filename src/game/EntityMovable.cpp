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
EntityMovable::SetMovePosition( const Ogre::Vector3& target )
{
    m_MovePosition = target;
}



const Ogre::Vector3&
EntityMovable::GetMovePosition() const
{
    return m_MovePosition;
}
