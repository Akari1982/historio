#include "../core/Logger.h"
#include "../core/Utilites.h"
#include "EntityManager.h"
#include "MapXmlFile.h"


MapXmlFile::MapXmlFile( const Ogre::String& file ):
    XmlFile( file )
{
}



MapXmlFile::~MapXmlFile()
{
}



void
MapXmlFile::LoadMap( MapSector& map_sector )
{
    TiXmlNode* node = m_File.RootElement();

    if( node == NULL || node->ValueStr() != "map" )
    {
        LOG_ERROR( "MapXmlFile: " + m_File.ValueStr() + " is not a valid map file! No <map> in root." );
        return;
    }

    node = node->FirstChild();
    int order = 0;
    while( node != NULL )
    {
        if( node->Type() == TiXmlNode::TINYXML_ELEMENT && node->ValueStr() == "tile" )
        {
            Ogre::String type = GetString( node, "type" );
            if( type == "GRASS" )
            {
                map_sector.Quad( order % 100, order / 100, 1, 1, 0.5f, 0.0f, 0.5f, 1.0f );
            }
            else if( type == "WATER" )
            {
                map_sector.Quad( order % 100, order / 100, 1, 1, 0.0f, 0.0f, 0.5f, 1.0f );
            }
            ++order;
        }
        node = node->NextSibling();
    }
}



void
MapXmlFile::LoadEntities()
{
    TiXmlNode* node = m_File.RootElement();

    if( node == NULL || node->ValueStr() != "map" )
    {
        LOG_ERROR( "MapXmlFile: " + m_File.ValueStr() + " is not a valid map file! No <map> in root." );
        return;
    }

    node = node->FirstChild();
    int order = 0;
    while( node != NULL )
    {
        if( node->Type() == TiXmlNode::TINYXML_ELEMENT && node->ValueStr() == "entity" )
        {
            Ogre::String name = GetString( node, "name" );
            Ogre::Vector2 pos = GetVector2( node, "pos" );
            EntityManager::getSingleton().AddEntityByName( name, pos.x, pos.y );
        }
        node = node->NextSibling();
    }
}
