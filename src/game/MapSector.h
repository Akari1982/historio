#ifndef MAP_SECTOR_H
#define MAP_SECTOR_H

#include <OgreCamera.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreSimpleRenderable.h>



struct MapTileDesc
{
    Ogre::String name;
    unsigned int collision_mask;
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

    void Quad( const unsigned int x, const unsigned int y, const float width, const float height, const Ogre::String& name );

    const int GetPass( const unsigned int x, const unsigned int y );

private:
    void CreateVertexBuffers();
    void DestroyVertexBuffers();
    void CreateMaterial();

private:
    std::vector< MapTileDesc > m_MapTileDescs;

    int m_PassMap[ 100 ][ 100 ];

    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;
    unsigned int m_MaxVertexCount;
};



#endif // MAP_SECTOR_H
