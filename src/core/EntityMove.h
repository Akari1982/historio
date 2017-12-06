#ifndef ENTITY_MOVE_H
#define ENTITY_MOVE_H

#include "Entity.h"



class EntityMove : public Entity
{
public:
    EntityMove();
    virtual ~EntityMove();

    virtual void Update();
    virtual void UpdateDebug();
};



#endif // ENTITY_MOVE_H
