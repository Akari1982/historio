#ifndef ENTITY_BUILDING_H
#define ENTITY_BUILDING_H

#include "Entity.h"



class EntityBuilding : public Entity
{
public:
    EntityBuilding();
    virtual ~EntityBuilding();

    virtual void Update();
    virtual void UpdateDebug();

protected:
    int m_Health;
};



#endif // ENTITY_BUILDING_H
