#ifndef ENTITY_H
#define ENTITY_H

#include "EntityTile.h"



class Entity : public EntityTile
{
public:
    Entity();
    virtual ~Entity();

    virtual void Update();
    virtual void UpdateDebug();

    void SetMovePosition( const Ogre::Vector2& target );
    const Ogre::Vector2& GetMovePosition() const;

private:
    Ogre::Vector2 m_MovePosition;
};



#endif // ENTITY_H
