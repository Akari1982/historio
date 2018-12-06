#ifndef XML_MAP_FILE_H
#define XML_MAP_FILE_H

#include "../core/XmlFile.h"
#include "Map.h"



class XmlMapFile : public XmlFile
{
public:
    XmlMapFile( const Ogre::String& file );
    virtual ~XmlMapFile();

    void LoadMap( Map* map );
};



#endif // XML_MAP_FILE_H
