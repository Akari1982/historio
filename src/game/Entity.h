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

protected:
    std::vector< Ogre::Vector3 > m_Occupation;
};



#endif // ENTITY_H
