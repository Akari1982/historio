#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <OgreRenderQueueListener.h>
#include <OgreSingleton.h>

#include "../core/Event.h"
#include "Entity.h"
#include "MapSector.h"

struct EntityDesc
{
    Ogre::String name;
    float width;
    float height;
}



class EntityManager : public Ogre::Singleton< EntityManager >, public Ogre::RenderQueueListener
{
public:
    EntityManager();
    virtual ~EntityManager();

    void InitCmd();

    void Input( const Event& event );
    void Update();
    void UpdateDebug();
    void OnResize();

    void AddEntityDesc( EntityDesc desc );

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

private:
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;

    MapSector m_MapSector;
    std::vector< EntityDesc > m_EntityDescs;
    std::vector< Entity* > m_Entities;
};



#endif // ENTITY_MANAGER_H
