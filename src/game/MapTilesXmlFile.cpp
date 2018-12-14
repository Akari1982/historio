#include "../core/Logger.h"
#include "../core/Utilites.h"
#include "EntityManager.h"
#include "MapTilesXmlFile.h"



MapTilesXmlFile::MapTilesXmlFile( const Ogre::String& file ):
    XmlFile( file )
{
}



MapTilesXmlFile::~MapTilesXmlFile()
{
}



void
MapTilesXmlFile::LoadDesc( MapSector* map_sector )
{
    TiXmlNode* node = m_File.RootElement();

    if( node == NULL || node->ValueStr() != "tiles" )
    {
        LOG_ERROR( "MapTilesXmlFile: " + m_File.ValueStr() + " is not a valid tiles description file! No <tiles> in root." );
        return;
    }

    node = node->FirstChild();
    while( node != NULL )
    {
        if( node->Type() == TiXmlNode::TINYXML_ELEMENT && node->ValueStr() == "tile" )
        {
            MapTileDesc desc;
            desc.name = GetString( node, "name" );
            desc.collision_mask = 0x0;
            Ogre::String collision_mask = GetString( node, "collision_mask" );
            Ogre::StringVector mask = Ogre::StringUtil::split( collision_mask, " " );
            for( unsigned int i = 0; i < mask.size(); ++i )
            {
                if( mask[ i ] == "unit" )
                {
                    desc.collision_mask |= 0x1;
                }
            }
            desc.colour = GetColourValue( node, "colour" );
            desc.texture_coords = GetVector4( node, "texture_coords", Ogre::Vector4( 0, 0, 1, 1 ) );
            map_sector->AddMapTileDesc( desc );
        }
        node = node->NextSibling();
    }
}
