#ifndef ENTITY_H
#define ENTITY_H

#include "EntityTile.h"



class Entity : public EntityTile
{
public:
    Entity( Ogre::SceneNode* node );
    virtual ~Entity();

    void SetOccupation( const std::vector< Ogre::Vector3 >& occupation );
    const std::vector< Ogre::Vector3 >& GetOccupation() const;

    void SetCollisionMask( const int mask );
    const int GetCollisionMask() const;

    enum Action
    {
        NONE,
        WALK,
        GATHER,
        ATTACK
    };
    void SetAction( const Action action );
    const Action GetAction() const;

protected:
    std::vector< Ogre::Vector3 > m_Occupation;
    int m_CollisionMask;
    Action m_Action;
};



#endif // ENTITY_H
