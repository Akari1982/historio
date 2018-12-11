#ifndef ENTITY_TILE_H
#define ENTITY_TILE_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderOperation.h>
#include <OgreVector3.h>
#include <OgreVector2.h>



class EntityTile
{
public:
    EntityTile();
    virtual ~EntityTile();

    void SetMaterial( const Ogre::MaterialPtr material );
    void SetPosition( const Ogre::Vector3& position );
    const Ogre::Vector3& GetPosition() const;
    void SetSize( const Ogre::Vector2& size );
    const Ogre::Vector2& GetSize() const;
    void SetTexture( const Ogre::String& texture );
    void SetColour( const Ogre::ColourValue& colour );
    void SetDepth( const float depth );
    void UpdateGeometry();

    void Render();

private:
    void CreateVertexBuffer();
    void DestroyVertexBuffer();

protected:
    Ogre::MaterialPtr m_Material;
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;
    Ogre::RenderOperation m_RenderOp;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;

    Ogre::Vector3 m_Position;
    Ogre::Vector2 m_Size;
    Ogre::ColourValue m_Colour;
    float m_Depth;
};



#endif // ENTITY_TILE_H
