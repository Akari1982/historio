#ifndef ENTITY_STAND_H
#define ENTITY_STAND_H

#include "Entity.h"



class EntityStand : public Entity
{
public:
    EntityStand( Ogre::SceneNode* node );
    virtual ~EntityStand();
};



#endif // ENTITY_STAND_H
