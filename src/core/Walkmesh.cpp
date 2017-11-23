#include "Logger.h"
#include "Walkmesh.h"

#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>





Walkmesh::Walkmesh()
{
    m_SceneManager = Ogre::Root::getSingletonPtr()->getSceneManager( "Scene" );
    m_RenderSystem = Ogre::Root::getSingleton().getRenderSystem();

    CreateVertexBuffers();

    m_Material = Ogre::MaterialManager::getSingleton().create( "Map", "General" );
    Ogre::Pass* pass = m_Material->getTechnique( 0 )->getPass( 0 );
    pass->setVertexColourTracking( Ogre::TVC_AMBIENT );
    pass->setCullingMode( Ogre::CULL_NONE );
    pass->setDepthCheckEnabled( true );
    pass->setDepthWriteEnabled( true );
    pass->setLightingEnabled( false );
    pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
    pass->setAlphaRejectFunction( Ogre::CMPF_GREATER );
    pass->setAlphaRejectValue( 0 );
    Ogre::TextureUnitState* tex = pass->createTextureUnitState();
    tex->setTextureName( "system/blank.png" );
    tex->setNumMipmaps( -1 );
    tex->setTextureFiltering( Ogre::TFO_NONE );

    m_SceneManager->addRenderQueueListener( this );

    for( int i = 0; i < 10; ++i )
    {
        for( int j = 0; j < 10; ++j )
        {
            //Quad( i * 10, j * 10, i * 10 + 10, j * 10, i * 10 + 10, j * 10 + 10, i * 10, j * 10 + 10 );
        }
    }
    Quad( 10, 10, 20, 10, 20, 20, 10, 20 );
    Quad( 0, 0, 10, 0, 10, 10, 0, 10 );
}



Walkmesh::~Walkmesh()
{
    m_SceneManager->removeRenderQueueListener( this );

    DestroyVertexBuffers();
}



void
Walkmesh::Quad( const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4 )
{
    if( m_RenderOp.vertexData->vertexCount + 6 > m_MaxVertexCount )
    {
        LOG_ERROR( "Walkmesh: Max number of quads reached. Can't create more than " + Ogre::StringConverter::toString( m_MaxVertexCount / 6 ) + " quads." );
        return;
    }

    float width = Ogre::Root::getSingleton().getRenderTarget( "QGearsWindow" )->getViewport( 0 )->getActualWidth();
    float height = Ogre::Root::getSingleton().getRenderTarget( "QGearsWindow" )->getViewport( 0 )->getActualHeight();

    float m_Z = 0.5f;
    Ogre::ColourValue m_Colour = Ogre::ColourValue::White;
    bool m_ScreenSpace = true;

    float new_x1 = ( m_ScreenSpace == true ) ? ( ( int ) x1 / width ) * 2 - 1 : x1;
    float new_y1 = ( m_ScreenSpace == true ) ? -( ( ( int ) y1 / height ) * 2 - 1 ) : y1;
    float new_x2 = ( m_ScreenSpace == true ) ? ( ( int ) x2 / width ) * 2 - 1 : x2;
    float new_y2 = ( m_ScreenSpace == true ) ? -( ( y2 / height ) * 2 - 1 ) : y2;
    float new_x3 = ( m_ScreenSpace == true ) ? ( ( int ) x3 / width ) * 2 - 1 : x3;
    float new_y3 = ( m_ScreenSpace == true ) ? -( ( ( int ) y3 / height ) * 2 - 1 ) : y3;
    float new_x4 = ( m_ScreenSpace == true ) ? ( ( int ) x4 / width ) * 2 - 1 : x4;
    float new_y4 = ( m_ScreenSpace == true ) ? -( ( ( int ) y4 / height ) * 2 - 1 ) : y4;

    float* writeIterator = ( float* ) m_VertexBuffer->lock( Ogre::HardwareBuffer::HBL_NORMAL );
    writeIterator += m_RenderOp.vertexData->vertexCount * 7;

    *writeIterator++ = new_x1;
    *writeIterator++ = new_y1;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;

    *writeIterator++ = new_x2;
    *writeIterator++ = new_y2;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;

    *writeIterator++ = new_x3;
    *writeIterator++ = new_y3;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;

    *writeIterator++ = new_x1;
    *writeIterator++ = new_y1;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;

    *writeIterator++ = new_x3;
    *writeIterator++ = new_y3;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;

    *writeIterator++ = new_x4;
    *writeIterator++ = new_y4;
    *writeIterator++ = m_Z;
    *writeIterator++ = m_Colour.r;
    *writeIterator++ = m_Colour.g;
    *writeIterator++ = m_Colour.b;
    *writeIterator++ = m_Colour.a;

    m_RenderOp.vertexData->vertexCount += 6;

    m_VertexBuffer->unlock();
}



void
Walkmesh::renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation )
{
    if( queueGroupId == Ogre::RENDER_QUEUE_MAIN )
    {
        m_RenderSystem->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
        m_RenderSystem->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );

        float width = Ogre::Root::getSingleton().getRenderTarget( "QGearsWindow" )->getViewport( 0 )->getActualWidth();
        float height = Ogre::Root::getSingleton().getRenderTarget( "QGearsWindow" )->getViewport( 0 )->getActualHeight();
        Ogre::Matrix4 view;
        //view.makeTrans( Ogre::Vector3( m_PositionReal.x * 2 / width * height / 720.0f, -m_PositionReal.y * 2 / 720.0f, 0 ) );
        //view.makeTrans( Ogre::Vector3( 0 * 2 / width * height / 720.0f, -20 * 2 / 720.0f, 0 ) );
        m_RenderSystem->_setViewMatrix( view );

        if( m_RenderOp.vertexData->vertexCount != 0 )
        {
            m_SceneManager->_setPass( m_Material->getTechnique( 0 )->getPass( 0 ), true, false );
            m_RenderSystem->_render( m_RenderOp );
        }
    }
}



void
Walkmesh::CreateVertexBuffers()
{
    m_MaxVertexCount = 2048 * 6;
    m_RenderOp.vertexData = new Ogre::VertexData;
    m_RenderOp.vertexData->vertexStart = 0;

    Ogre::VertexDeclaration* vDecl = m_RenderOp.vertexData->vertexDeclaration;

    size_t offset = 0;
    vDecl->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT4 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

    m_VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer( vDecl->getVertexSize( 0 ), m_MaxVertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY, false );

    m_RenderOp.vertexData->vertexBufferBinding->setBinding( 0, m_VertexBuffer );
    m_RenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    m_RenderOp.useIndexes = false;
}



void
Walkmesh::DestroyVertexBuffers()
{
    delete m_RenderOp.vertexData;
    m_RenderOp.vertexData = 0;
    m_VertexBuffer.setNull();
    m_MaxVertexCount = 0;
}
