#ifndef MAP_H
#define MAP_H

#include <OgreRenderQueueListener.h>

#include "MapSector.h"



struct MapTile
{
    enum MapTileType
    {
        GRASS = 0,
        WATER = 1,
    };

    MapTileType type;
};



class Map : public Ogre::RenderQueueListener
{
public:
    Map();
    virtual ~Map();

    MapTile GetTile( const unsigned int x, const unsigned int y );

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

private:
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;

    MapTile m_MapData[ 10000 ];
    MapSector* m_MapSector;
};



#endif // MAP_H
