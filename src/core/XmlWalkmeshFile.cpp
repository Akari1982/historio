#include "XmlWalkmeshFile.h"

#include "EntityManager.h"
#include "Logger.h"
#include "Utilites.h"
#include "Walkmesh.h"



XmlWalkmeshFile::XmlWalkmeshFile( const Ogre::String& file ):
    XmlFile( file )
{
}



XmlWalkmeshFile::~XmlWalkmeshFile()
{
}



void
XmlWalkmeshFile::Load()
{
    Walkmesh* walkmesh = new Walkmesh();
    EntityManager::getSingleton().AddWalkmesh( walkmesh );
}
