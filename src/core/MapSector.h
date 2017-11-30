#ifndef MAP_SECTOR_H
#define MAP_SECTOR_H

class Map;
class Tile;



class MapSector
{
public:
    MapSector( Map* map );
    virtual ~MapSector();

private:
    Map* m_Map;
    std::vector< Tile* > m_Tiles;
};



#endif // MAP_SECTOR_H
