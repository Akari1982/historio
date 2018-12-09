#ifndef MAP_SECTOR_H
#define MAP_SECTOR_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreRenderOperation.h>



class MapSector
{
public:
    MapSector();
    virtual ~MapSector();

    void Render();

    void Quad( const float x, const float y, const float width, const float height, const float u, const float v, const float tw, const float th, const Ogre::ColourValue& colour =  Ogre::ColourValue( 1, 1, 1, 1 ) );

private:
    void CreateVertexBuffers();
    void DestroyVertexBuffers();
    void CreateMaterial();

private:
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;

    Ogre::RenderOperation m_RenderOp;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;
    unsigned int m_MaxVertexCount;
    Ogre::MaterialPtr m_Material;
};



#endif // MAP_SECTOR_H
