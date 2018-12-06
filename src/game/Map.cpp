#include "Map.h"

#include "XmlMapFile.h"



Map::Map()
{
    XmlMapFile* map_loader = new XmlMapFile( "data/map/test.xml" );
    map_loader->LoadMap( this );
    delete map_loader;

    m_MapSector = new MapSector( this );
}



Map::~Map()
{
    delete m_MapSector;
}



void
Map::AddTile( const unsigned int x, const unsigned int y, MapTile tile )
{
    if( y * 100 + x < 10000 )
    {
        m_MapData[ y * 100 + x ] = tile;
    }
}



MapTile
Map::GetTile( const unsigned int x, const unsigned int y )
{
    return m_MapData[ y * 100 + x ];
}
