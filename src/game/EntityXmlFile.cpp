#include "EntityXmlFile.h"

#include "../core/Logger.h"
#include "../core/Utilites.h"
#include "EntityManager.h"



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
            Ogre::String name = GetString( node, "name" );
            Ogre::Vector2 size = GetVector2( node, "size", Ogre::Vector2( 1, 1 ) );
            Ogre::String texture = GetString( node, "texture" );
            EntityDesc desc;
            desc.name = name;
            desc.width = size.x;
            desc.height = size.y;
            desc.texture = texture;
            EntityManager::getSingleton().AddEntityDesc( desc );
        }
        node = node->NextSibling();
    }
}
