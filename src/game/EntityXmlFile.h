#ifndef ENTITY_XML_FILE_H
#define ENTITY_XML_FILE_H

#include "../core/XmlFile.h"



class EntityXmlFile : public XmlFile
{
public:
    EntityXmlFile( const Ogre::String& file );
    virtual ~EntityXmlFile();

    void LoadDesc();
};



#endif // ENTITY_XML_FILE_H
