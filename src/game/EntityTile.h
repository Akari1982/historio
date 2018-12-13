#ifndef ENTITY_TILE_H
#define ENTITY_TILE_H

#include <OgreCamera.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreSimpleRenderable.h>
#include <OgreVector3.h>
#include <OgreVector4.h>



class EntityTile : public Ogre::SimpleRenderable
{
public:
    EntityTile( Ogre::SceneNode* node );
    virtual ~EntityTile();

    Ogre::Real getSquaredViewDepth( const Ogre::Camera* cam ) const;
    Ogre::Real getBoundingRadius() const;

    void SetMaterial( const Ogre::MaterialPtr material );
    void SetPosition( const Ogre::Vector3& position );
    const Ogre::Vector3& GetPosition() const;
    void SetCollisionBox( const Ogre::Vector4& сollision_box );
    const Ogre::Vector4& GetCollisionBox() const;
    void SetDrawBox( const Ogre::Vector4& draw_box );
    const Ogre::Vector4& GetDrawBox() const;
    void SetTexture( const Ogre::String& texture );
    void SetColour( const Ogre::ColourValue& colour );
    void SetDepth( const float depth );
    void UpdateGeometry();

private:
    EntityTile();

    void CreateVertexBuffer();
    void DestroyVertexBuffer();

protected:
    Ogre::SceneNode* m_SceneNode;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;

    Ogre::Vector4 m_CollisionBox;
    Ogre::Vector4 m_DrawBox;
    Ogre::ColourValue m_Colour;
    float m_Depth;
};



#endif // ENTITY_TILE_H
