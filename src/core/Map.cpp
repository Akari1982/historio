#include "Map.h"



Map::Map()
{
    for( int i = 0; i < 100; ++i )
    {
        for( int j = 0; j < 100; ++j )
        {
            MapTile tile;
            if( i > 20 )
            {
                tile.type = MapTile::GRASS;
            }
            else
            {
                tile.type = MapTile::WATER;
            }
            m_MapData[ i * 100 + j ] = tile;
        }
    }

    m_MapSector = new MapSector( this );
}



Map::~Map()
{
    delete m_MapSector;
}



MapTile
Map::GetTile( const unsigned int x, const unsigned int y )
{
    return m_MapData[ y * 100 + x ];
}



void
Map::Render()
{
    m_MapSector->Render();
}
