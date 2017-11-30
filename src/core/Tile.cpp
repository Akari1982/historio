#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>

#include "CameraManager.h"
#include "DebugDraw.h"
#include "Logger.h"
#include "Tile.h"



Tile::Tile():
    m_X( 0 ),
    m_Y( 0 ),
    m_Width( 1 ),
    m_Height( 1 ),
    m_Colour( Ogre::ColourValue( 1, 1, 1, 1 ) )
{
    m_SceneManager = Ogre::Root::getSingletonPtr()->getSceneManager( "Scene" );
    m_RenderSystem = Ogre::Root::getSingleton().getRenderSystem();

    CreateVertexBuffer();

    m_SceneManager->addRenderQueueListener( this );
}



Tile::~Tile()
{
    m_SceneManager->removeRenderQueueListener( this );

    DestroyVertexBuffer();
}



void
Tile::SetMaterial( const Ogre::MaterialPtr material )
{
    m_Material = material;
}



void
Tile::SetPosition( const float x, const float y )
{
    m_X = x;
    m_Y = y;
}



void
Tile::SetSize( const float width, const float height )
{
    m_Width = width;
    m_Height = height;
}



void
Tile::SetColour( const Ogre::ColourValue& colour )
{
    m_Colour = colour;
}



void
Tile::UpdateGeometry()
{
    float x1 = m_X;
    float y1 = m_Y;
    float x2 = m_X + m_Width;
    float y2 = m_Y;
    float x3 = m_X + m_Width;
    float y3 = m_Y + m_Height;
    float x4 = m_X;
    float y4 = m_Y + m_Height;

    float left = 0.0f;
    float right = 1.0f;
    float top = 0.0f;
    float bottom = 1.0f;

    float m_Z = 0.5f;

    float* writeIterator = ( float* ) m_VertexBuffer->lock( Ogre::HardwareBuffer::HBL_NORMAL );

    *writeIterator++ = x1;
    *writeIterator++ = y1;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = left;
    *writeIterator++ = top;

    *writeIterator++ = x2;
    *writeIterator++ = y2;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = right;
    *writeIterator++ = top;

    *writeIterator++ = x3;
    *writeIterator++ = y3;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = right;
    *writeIterator++ = bottom;

    *writeIterator++ = x1;
    *writeIterator++ = y1;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = left;
    *writeIterator++ = top;

    *writeIterator++ = x3;
    *writeIterator++ = y3;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;
    *writeIterator++ = right;
    *writeIterator++ = bottom;

    *writeIterator++ = x4;
    *writeIterator++ = y4;
    *writeIterator++ = m_Z;
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
Tile::renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation )
{
    if( queueGroupId == Ogre::RENDER_QUEUE_MAIN )
    {
        m_RenderSystem->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
        m_RenderSystem->_setViewMatrix( CameraManager::getSingleton().GetCurrentCamera()->getViewMatrix( true ) );
        m_RenderSystem->_setProjectionMatrix( CameraManager::getSingleton().GetCurrentCamera()->getProjectionMatrixRS() );

        if( m_Material.isNull() == false )
        {
            m_SceneManager->_setPass( m_Material->getTechnique( 0 )->getPass( 0 ), true, false );
            m_RenderSystem->_render( m_RenderOp );
        }
    }
}



void
Tile::CreateVertexBuffer()
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
Tile::DestroyVertexBuffer()
{
    delete m_RenderOp.vertexData;
    m_RenderOp.vertexData = 0;
    m_VertexBuffer.setNull();
}
