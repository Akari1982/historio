#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>
#include <OgreRoot.h>
#include "EntityTile.h"



EntityTile::EntityTile( Ogre::SceneNode* node ):
    m_SceneNode( node ),
    m_CollisionBox( Ogre::Vector4::ZERO ),
    m_DrawBox( Ogre::Vector4::ZERO ),
    m_Colour( Ogre::ColourValue( 1, 1, 1, 1 ) ),
    m_Depth( 0.0f )
{
    CreateVertexBuffer();

    Ogre::AxisAlignedBox aabb;
    aabb.setInfinite();
    setBoundingBox( aabb );
}



EntityTile::~EntityTile()
{
    DestroyVertexBuffer();
}



Ogre::Real
EntityTile::getSquaredViewDepth( const Ogre::Camera* cam ) const
{
    return 0;
}



Ogre::Real
EntityTile::getBoundingRadius() const
{
    return 0;
}



void
EntityTile::SetMaterial( const Ogre::MaterialPtr material )
{
    mMaterial = material;
}



void
EntityTile::SetPosition( const Ogre::Vector3& position )
{
    m_SceneNode->setPosition( position );
}



const Ogre::Vector3&
EntityTile::GetPosition() const
{
    return m_SceneNode->getPosition();
}



void
EntityTile::SetCollisionBox( const Ogre::Vector4& collision_box )
{
    m_CollisionBox = collision_box;
}



const Ogre::Vector4&
EntityTile::GetCollisionBox() const
{
    return m_CollisionBox;
}



void
EntityTile::SetDrawBox( const Ogre::Vector4& draw_box )
{
    m_DrawBox = draw_box;
}



const Ogre::Vector4&
EntityTile::GetDrawBox() const
{
    return m_DrawBox;
}



void
EntityTile::SetTexture( const Ogre::String& texture )
{
    Ogre::Pass* pass = mMaterial->getTechnique( 0 )->getPass( 0 );
    Ogre::TextureUnitState* tex = pass->getTextureUnitState( 0 );
    tex->setTextureName( texture );
}



void
EntityTile::SetColour( const Ogre::ColourValue& colour )
{
    m_Colour = colour;
}



void
EntityTile::SetDepth( const float depth )
{
    m_Depth = depth;
}



void
EntityTile::UpdateGeometry()
{
    float x1 = m_DrawBox.x;
    float y1 = m_DrawBox.y;
    float x2 = m_DrawBox.z;
    float y2 = m_DrawBox.y;
    float x3 = m_DrawBox.z;
    float y3 = m_DrawBox.w;
    float x4 = m_DrawBox.x;
    float y4 = m_DrawBox.w;

    float left = 0.0f;
    float right = 1.0f;
    float top = 0.0f;
    float bottom = 1.0f;

    float* writeIterator = ( float* ) m_VertexBuffer->lock( Ogre::HardwareBuffer::HBL_NORMAL );

    *writeIterator++ = x1;
    *writeIterator++ = y1;
    *writeIterator++ = m_Depth;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = left;
    *writeIterator++ = top;

    *writeIterator++ = x2;
    *writeIterator++ = y2;
    *writeIterator++ = m_Depth;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = right;
    *writeIterator++ = top;

    *writeIterator++ = x3;
    *writeIterator++ = y3;
    *writeIterator++ = m_Depth;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = right;
    *writeIterator++ = bottom;

    *writeIterator++ = x1;
    *writeIterator++ = y1;
    *writeIterator++ = m_Depth;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = left;
    *writeIterator++ = top;

    *writeIterator++ = x3;
    *writeIterator++ = y3;
    *writeIterator++ = m_Depth;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = right;
    *writeIterator++ = bottom;

    *writeIterator++ = x4;
    *writeIterator++ = y4;
    *writeIterator++ = m_Depth;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = left;
    *writeIterator++ = bottom;

    mRenderOp.vertexData->vertexCount = 6;

    m_VertexBuffer->unlock();
}



void
EntityTile::CreateVertexBuffer()
{
    mRenderOp.vertexData = new Ogre::VertexData;
    mRenderOp.vertexData->vertexStart = 0;

    Ogre::VertexDeclaration* vDecl = mRenderOp.vertexData->vertexDeclaration;

    size_t offset = 0;
    vDecl->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT4 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

    m_VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer( vDecl->getVertexSize( 0 ), 6, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY, false );

    mRenderOp.vertexData->vertexBufferBinding->setBinding( 0, m_VertexBuffer );
    mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    mRenderOp.useIndexes = false;
}



void
EntityTile::DestroyVertexBuffer()
{
    delete mRenderOp.vertexData;
    mRenderOp.vertexData = 0;
    m_VertexBuffer.setNull();
}
