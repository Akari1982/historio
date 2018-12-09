#ifndef MAP_XML_FILE_H
#define MAP_XML_FILE_H

#include "../core/XmlFile.h"
#include "MapSector.h"



class MapXmlFile : public XmlFile
{
public:
    MapXmlFile( const Ogre::String& file );
    virtual ~MapXmlFile();

    void LoadMap( MapSector& map_sector );
    void LoadEntities();
};



#endif // MAP_XML_FILE_H
