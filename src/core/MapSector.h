#ifndef MAP_SECTOR_H
#define MAP_SECTOR_H

#include "Tile.h"



class MapSector
{
public:
    MapSector();
    virtual ~MapSector();

private:
    std::vector< Tile* > m_Tiles;
};



#endif // MAP_SECTOR_H
