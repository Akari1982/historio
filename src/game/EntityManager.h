#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

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
    unsigned int collision_mask;
    Ogre::Vector4 collision_box;
    Ogre::Vector4 draw_box;
    Ogre::String texture;
};



class EntityManager : public Ogre::Singleton< EntityManager >
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

    void SetEntitySelection( const Ogre::Vector3& start, const Ogre::Vector3& end );
    void SetEntitySelectionMove( const Ogre::Vector3& move );

private:
    struct AStarNode
    {
        int x;
        int y;
        int pass;
        float g;
        float h;
        float f;
        bool opened;
        bool closed;
        AStarNode* parent;
    };
    std::vector< Ogre::Vector3 > AStarFinder( Entity* entity, const int start_x, const int start_y, const int end_x, const int end_y ) const;
    const Ogre::Vector3 PlaceFinder( Entity* entity, const int x, const int y ) const;
    const bool IsPassable( Entity* entity, const int x, const int y ) const;

private:
    Ogre::SceneManager* m_SceneManager;
    Ogre::SceneNode* m_SceneNode;

    HudManager* m_Hud;

    MapSector m_MapSector;
    std::vector< EntityDesc > m_EntityDescs;
    std::vector< Entity* > m_Entities;
    std::vector< EntityMovable* > m_EntitiesMovable;
    std::vector< EntityMovable* > m_EntitiesSelected;
};



#endif // ENTITY_MANAGER_H
