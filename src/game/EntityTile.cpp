#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>
#include <OgreRoot.h>

#include "EntityTile.h"



EntityTile::EntityTile():
    m_Position( Ogre::Vector2::ZERO ),
    m_Size( Ogre::Vector2( 1, 1 ) ),
    m_Colour( Ogre::ColourValue( 1, 1, 1, 1 ) ),
    m_Depth( 0.0f )
{
    m_SceneManager = Ogre::Root::getSingletonPtr()->getSceneManager( "Scene" );
    m_RenderSystem = Ogre::Root::getSingleton().getRenderSystem();

    CreateVertexBuffer();
}



EntityTile::~EntityTile()
{
    DestroyVertexBuffer();
}



void
EntityTile::SetMaterial( const Ogre::MaterialPtr material )
{
    m_Material = material;
}



void
EntityTile::SetPosition( const Ogre::Vector2& position )
{
    m_Position = position;
    UpdateGeometry();
}



const Ogre::Vector2&
EntityTile::GetPosition() const
{
    return m_Position;
}



void
EntityTile::SetSize( const Ogre::Vector2& size )
{
    m_Size = size;
}



const Ogre::Vector2&
EntityTile::GetSize() const
{
    return m_Size;
}



void
EntityTile::SetTexture( const Ogre::String& texture )
{
    Ogre::Pass* pass = m_Material->getTechnique( 0 )->getPass( 0 );
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
    float x1 = m_Position.x;
    float y1 = m_Position.y;
    float x2 = m_Position.x + m_Size.x;
    float y2 = m_Position.y;
    float x3 = m_Position.x + m_Size.x;
    float y3 = m_Position.y + m_Size.y;
    float x4 = m_Position.x;
    float y4 = m_Position.y + m_Size.y;

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

    m_RenderOp.vertexData->vertexCount = 6;

    m_VertexBuffer->unlock();
}



void
EntityTile::Render()
{
    m_SceneManager->_setPass( m_Material->getTechnique( 0 )->getPass( 0 ), true, false );
    m_RenderSystem->_render( m_RenderOp );
}



void
EntityTile::CreateVertexBuffer()
{
    m_RenderOp.vertexData = new Ogre::VertexData;
    m_RenderOp.vertexData->vertexStart = 0;

    Ogre::VertexDeclaration* vDecl = m_RenderOp.vertexData->vertexDeclaration;

    size_t offset = 0;
    vDecl->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT4 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

    m_VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer( vDecl->getVertexSize( 0 ), 6, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY, false );

    m_RenderOp.vertexData->vertexBufferBinding->setBinding( 0, m_VertexBuffer );
    m_RenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    m_RenderOp.useIndexes = false;
}



void
EntityTile::DestroyVertexBuffer()
{
    delete m_RenderOp.vertexData;
    m_RenderOp.vertexData = 0;
    m_VertexBuffer.setNull();
}
