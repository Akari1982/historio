#include "../core/Logger.h"
#include "../core/Utilites.h"
#include "EntityManager.h"
#include "EntityXmlFile.h"



EntityXmlFile::EntityXmlFile( const Ogre::String& file ):
    XmlFile( file )
{
}



EntityXmlFile::~EntityXmlFile()
{
}



void
EntityXmlFile::LoadDesc()
{
    TiXmlNode* node = m_File.RootElement();

    if( node == NULL || node->ValueStr() != "entities" )
    {
        LOG_ERROR( "EntityXmlFile: " + m_File.ValueStr() + " is not a valid entities description file! No <entities> in root." );
        return;
    }

    node = node->FirstChild();
    while( node != NULL )
    {
        if( node->Type() == TiXmlNode::TINYXML_ELEMENT && node->ValueStr() == "entity" )
        {
            EntityDesc desc;
            desc.entity_class = GetString( node, "class" );
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
            desc.collision_box = GetVector4( node, "collision_box", Ogre::Vector4( 0, 0, 1, 1 ) );
            desc.draw_box = GetVector4( node, "draw_box", Ogre::Vector4( 0, 0, 1, 1 ) );
            desc.texture = GetString( node, "texture" );
            EntityManager::getSingleton().AddEntityDesc( desc );
        }
        node = node->NextSibling();
    }
}
