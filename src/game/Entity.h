#ifndef ENTITY_H
#define ENTITY_H

#include "EntityTile.h"



class Entity : public EntityTile
{
public:
    Entity( Ogre::SceneNode* node );
    virtual ~Entity();

    virtual void Update();
    virtual void UpdateDebug();

    const std::vector< Ogre::Vector3 >& GetOcupation() const;

private:
    std::vector< Ogre::Vector3 > m_Occupation;
};



#endif // ENTITY_H
