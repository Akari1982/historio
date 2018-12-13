#ifndef MAP_TILES_XML_FILE_H
#define MAP_TILES_XML_FILE_H

#include "../core/XmlFile.h"
#include "MapSector.h"



class MapTilesXmlFile : public XmlFile
{
public:
    MapTilesXmlFile( const Ogre::String& file );
    virtual ~MapTilesXmlFile();

    void LoadDesc( MapSector* map_sector );
};



#endif // MAP_TILES_XML_FILE_H
