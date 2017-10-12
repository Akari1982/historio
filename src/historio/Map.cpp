#include "Map.h"

#include "../core/ConfigVar.h"
#include "../core/DebugDraw.h"
#include "../core/Logger.h"



ConfigVar cv_debug_map( "debug_map", "Draw map and pass info", "false" );



Map::Map()
{
    for( int i = 0; i < 1000; ++i )
    {
        for( int j = 0; j < 1000; ++j )
        {
            block.pass[ i ][ j ] = false;
        }
    }
}



Map::~Map()
{
}



void
Map::DrawDebug()
{
    if( cv_debug_map.GetB() == true )
    {
        DEBUG_DRAW.SetScreenSpace( true );
        DEBUG_DRAW.SetColour( Ogre::ColourValue( 1, 0, 0, 1 ) );

        for( int i = 0; i < 10; ++i )
        {
            for( int j = 0; j < 10; ++j )
            {
                DEBUG_DRAW.Line( i * 10, j * 10, i * 10 + 10, j * 10 );
                DEBUG_DRAW.Line( i * 10 + 10, j * 10, i * 10 + 10, j * 10 + 10 );
                DEBUG_DRAW.Line( i * 10 + 10, j * 10 + 10, i * 10, j * 10 + 10  );
                DEBUG_DRAW.Line( i * 10, j * 10 + 10, i * 10, j * 10 );
            }
        }
    }
}
