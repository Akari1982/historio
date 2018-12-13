#ifndef MAP_SECTOR_H
#define MAP_SECTOR_H

#include <OgreCamera.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreSimpleRenderable.h>



struct MapTileDesc
{
    Ogre::String name;
    Ogre::ColourValue colour;
    Ogre::Vector4 texture_coords;
};



class MapSector : public Ogre::SimpleRenderable
{
public:
    MapSector();
    virtual ~MapSector();

    Ogre::Real getSquaredViewDepth( const Ogre::Camera* cam ) const;
    Ogre::Real getBoundingRadius() const;

    void AddMapTileDesc( const MapTileDesc& desc );

    void Quad( const float x, const float y, const float width, const float height, const Ogre::String& name );

private:
    void CreateVertexBuffers();
    void DestroyVertexBuffers();
    void CreateMaterial();

private:
    std::vector< MapTileDesc > m_MapTileDescs;

    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;
    unsigned int m_MaxVertexCount;
};



#endif // MAP_SECTOR_H
