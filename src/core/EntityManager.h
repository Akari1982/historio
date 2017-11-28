#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <OgreSingleton.h>

#include "Event.h"
#include "Entity.h"
#include "Map.h"



class EntityManager : public Ogre::Singleton< EntityManager >
{
public:
    EntityManager();
    virtual ~EntityManager();

    void Input( const Event& event );
    void Update();
    void UpdateDebug();
    void OnResize();

    void InitCmd();

private:
    bool m_Paused;

    Map m_Map;

    std::vector< Entity* > m_Entity;
};



#endif // ENTITY_MANAGER_H
