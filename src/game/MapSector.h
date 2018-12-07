#ifndef MAP_SECTOR_H
#define MAP_SECTOR_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreRenderOperation.h>



class MapSector : public Ogre::RenderQueueListener
{
public:
    MapSector();
    virtual ~MapSector();

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

    void Quad( const float x, const float y, const float width, const float height, const Ogre::ColourValue& colour );

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
