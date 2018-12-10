#ifndef ENTITY_MOVABLE_H
#define ENTITY_MOVABLE_H

#include "Entity.h"



class EntityMovable : public Entity
{
public:
    EntityMovable();
    virtual ~EntityMovable();

    virtual void Update();
    virtual void UpdateDebug();

    void SetMovePosition( const Ogre::Vector2& target );
    const Ogre::Vector2& GetMovePosition() const;

private:
    Ogre::Vector2 m_MovePosition;
};



#endif // ENTITY_MOVABLE_H
