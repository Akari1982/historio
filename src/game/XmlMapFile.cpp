#include "XmlMapFile.h"

#include "../core/Logger.h"
#include "../core/TextManager.h"
#include "../core/Utilites.h"



XmlMapFile::XmlMapFile( const Ogre::String& file ):
    XmlFile( file )
{
}



XmlMapFile::~XmlMapFile()
{
}



void
XmlMapFile::LoadMap( Map* map )
{
    TiXmlNode* node = m_File.RootElement();

    if( node == NULL || node->ValueStr() != "map" )
    {
        LOG_ERROR( "Text Manager: " + m_File.ValueStr() + " is not a valid map file! No <map> in root." );
        return;
    }

    node = node->FirstChild();
    int order = 0;
    while( node != NULL )
    {
        if( node->Type() == TiXmlNode::TINYXML_ELEMENT && node->ValueStr() == "tile" )
        {
            Ogre::String type = GetString( node, "type" );
            MapTile tile;
            if( type == "GRASS" )
            {
                tile.type = MapTile::GRASS;
            }
            else
            {
                tile.type = MapTile::WATER;
            }
            map->AddTile( order % 100, order / 100, tile );
            ++order;
        }
        node = node->NextSibling();
    }
}
