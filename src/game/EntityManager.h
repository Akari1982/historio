#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <OgreRenderQueueListener.h>
#include <OgreSingleton.h>
#include "../core/Event.h"
#include "EntityMovable.h"
#include "EntityStand.h"
#include "HudManager.h"
#include "MapSector.h"



struct EntityDesc
{
    Ogre::String entity_class;
    Ogre::String name;
    float width;
    float height;
    Ogre::String texture;
};



class EntityManager : public Ogre::Singleton< EntityManager >, public Ogre::RenderQueueListener
{
public:
    EntityManager();
    virtual ~EntityManager();

    void InitCmd();

    void Input( const Event& event );
    void Update();
    void UpdateDebug();

    void AddEntityByName( const Ogre::String& name, const float x, const float y );
    void AddEntityDesc( const EntityDesc& desc );

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

private:
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;

    HudManager* m_Hud;

    MapSector m_MapSector;
    std::vector< EntityDesc > m_EntityDescs;
    std::vector< Entity* > m_Entities;
    std::vector< EntityMovable* > m_EntitiesMovable;
};



#endif // ENTITY_MANAGER_H
