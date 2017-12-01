#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <OgreRenderQueueListener.h>
#include <OgreSingleton.h>

#include "Event.h"
#include "Entity.h"
#include "Map.h"



class EntityManager : public Ogre::Singleton< EntityManager >, public Ogre::RenderQueueListener
{
public:
    EntityManager();
    virtual ~EntityManager();

    void Input( const Event& event );
    void Update();
    void UpdateDebug();
    void OnResize();

    void InitCmd();

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

private:
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;

    Map m_Map;
    std::vector< Entity* > m_Entity;
};



#endif // ENTITY_MANAGER_H
