#ifndef WALKMESH_H
#define WALKMESH_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreRoot.h>



class Walkmesh : public Ogre::RenderQueueListener
{
public:
    Walkmesh();
    virtual ~Walkmesh();

    void Quad( const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4 );

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

private:
    void CreateVertexBuffers();
    void DestroyVertexBuffers();

private:
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;

    Ogre::RenderOperation m_RenderOp;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;
    unsigned int m_MaxVertexCount;
    Ogre::MaterialPtr m_Material;
};



#endif // WALKMESH_H
