#ifndef MAP_H
#define MAP_H

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



class Map
{
public:
    Map();
    virtual ~Map();

    MapTile GetTile( const unsigned int x, const unsigned int y );

    void Render();

private:
    MapTile m_MapData[ 10000 ];
    MapSector* m_MapSector;
};



#endif // MAP_H
