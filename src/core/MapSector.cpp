#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

#include "Logger.h"
#include "Map.h"
#include "MapSector.h"
#include "Tile.h"



MapSector::MapSector( Map* map ):
    m_Map( map )
{
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create( "Map", "General" );
    Ogre::Pass* pass = material->getTechnique( 0 )->getPass( 0 );
    pass->setVertexColourTracking( Ogre::TVC_AMBIENT );
    pass->setCullingMode( Ogre::CULL_NONE );
    pass->setDepthCheckEnabled( true );
    pass->setDepthWriteEnabled( true );
    pass->setLightingEnabled( false );
    //pass->setPolygonMode( Ogre::PolygonMode::PM_WIREFRAME );
    pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
    pass->setAlphaRejectFunction( Ogre::CMPF_GREATER );
    pass->setAlphaRejectValue( 0 );
    Ogre::TextureUnitState* tex = pass->createTextureUnitState();
    tex->setTextureName( "system/blank.png" );
    tex->setNumMipmaps( -1 );
    tex->setTextureFiltering( Ogre::TFO_NONE );

    for( int i = 0; i < 100; ++i )
    {
        for( int j = 0; j < 100; ++j )
        {
            Tile* tile = new Tile();
            tile->SetPosition( i, j );
            tile->SetSize( 1, 1 );
            MapTile map_tile = m_Map->GetTile( i, j );
            if( map_tile.type == MapTile::GRASS )
            {
                tile->SetColour( Ogre::ColourValue( 0, 1, 0, 1 ) );
            }
            else if( map_tile.type == MapTile::WATER )
            {
                tile->SetColour( Ogre::ColourValue( 0, 0, 1, 1 ) );
            }
            tile->SetMaterial( material );
            tile->UpdateGeometry();
            m_Tiles.push_back( tile );
        }
    }
}



MapSector::~MapSector()
{
    for( unsigned int i = 0; i < m_Tiles.size(); ++i )
    {
        delete m_Tiles[ i ];
    }
    m_Tiles.clear();
}



void
MapSector::Render()
{
    for( unsigned int i = 0; i < m_Tiles.size(); ++i )
    {
        m_Tiles[ i ]->Render();
    }
}
