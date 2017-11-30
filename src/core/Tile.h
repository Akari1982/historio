#ifndef TILE_H
#define TILE_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreRenderOperation.h>
#include <OgreString.h>



class Tile : public Ogre::RenderQueueListener
{
public:
    Tile();
    virtual ~Tile();

    void SetMaterial( const Ogre::MaterialPtr material );
    void SetPosition( const float x, const float y );
    void SetSize( const float width, const float height );
    void SetColour( const Ogre::ColourValue& colour );
    void UpdateGeometry();

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

private:
    void CreateVertexBuffer();
    void DestroyVertexBuffer();

protected:
    Ogre::MaterialPtr m_Material;
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;
    Ogre::RenderOperation m_RenderOp;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;

    float m_X;
    float m_Y;
    float m_Width;
    float m_Height;
    Ogre::ColourValue m_Colour
};



#endif // TILE_H
