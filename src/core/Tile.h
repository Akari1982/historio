#ifndef TILE_H
#define TILE_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderOperation.h>
#include <OgreVector2.h>



class Tile
{
public:
    Tile();
    virtual ~Tile();

    void SetMaterial( const Ogre::MaterialPtr material );
    void SetPosition( const Ogre::Vector2& position );
    const Ogre::Vector2& GetPosition() const;
    void SetSize( const Ogre::Vector2& size );
    const Ogre::Vector2& GetSize() const;
    void SetColour( const Ogre::ColourValue& colour );
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

    Ogre::Vector2 m_Position;
    Ogre::Vector2 m_Size;
    Ogre::ColourValue m_Colour;
};



#endif // TILE_H
