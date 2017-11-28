#include "CameraManager.h"
#include "ConfigVar.h"
#include "DebugDraw.h"
#include "Entity.h"
#include "Logger.h"

#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>



ConfigVar cv_debug_entity( "debug_entity", "Draw entity debug info", "0" );



Entity::Entity()
{
    m_SceneManager = Ogre::Root::getSingletonPtr()->getSceneManager( "Scene" );
    m_RenderSystem = Ogre::Root::getSingleton().getRenderSystem();

    CreateVertexBuffer();
    CreateMaterial();

    UpdateGeometry();

    m_SceneManager->addRenderQueueListener( this );
}



Entity::~Entity()
{
    m_SceneManager->removeRenderQueueListener( this );

    DestroyVertexBuffer();
}



void
Entity::SetImage( const Ogre::String& image )
{
    Ogre::Pass* pass = m_Material->getTechnique( 0 )->getPass( 0 );
    Ogre::TextureUnitState* tex = pass->getTextureUnitState( 0 );
    tex->setTextureName( image );
}



void
Entity::UpdateDebug()
{
/*
    int debug = cv_debug_entity.GetI();

    // debug output
    if( debug > 0 )
    {
        DEBUG_DRAW.SetColour( Ogre::ColourValue::White );
        DEBUG_DRAW.SetScreenSpace( true );
        DEBUG_DRAW.SetTextAlignment( DEBUG_DRAW.CENTER );
        DEBUG_DRAW.SetFadeDistance( 40, 50 );

        Ogre::Vector3 entity_pos = GetPosition();

        DEBUG_DRAW.Text( entity_pos, 0, 0, m_Name );
        DEBUG_DRAW.Text( entity_pos, 0, 24, Ogre::StringConverter::toString( entity_pos ) + " (" + Ogre::StringConverter::toString( m_TriangleId ) + ")" );
    }
*/
}


void
Entity::UpdateGeometry()
{
    float x = 0;
    float y = 0;
    float width = 20;
    float height = 10;

    float x1 = x;
    float y1 = y;
    float x2 = x + width;
    float y2 = y;
    float x3 = x + width;
    float y3 = y + height;
    float x4 = x;
    float y4 = y + height;

    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;

    float m_Z = 0.5f;

    Ogre::ColourValue colour = Ogre::ColourValue( 1, 1, 1, 1 );

    float* writeIterator = ( float* ) m_VertexBuffer->lock( Ogre::HardwareBuffer::HBL_NORMAL );

    *writeIterator++ = x1;
    *writeIterator++ = y1;
    *writeIterator++ = m_Z;
    *writeIterator++ = colour.r;
    *writeIterator++ = colour.g;
    *writeIterator++ = colour.b;
    *writeIterator++ = colour.a;
    *writeIterator++ = left;
    *writeIterator++ = top;

    *writeIterator++ = x2;
    *writeIterator++ = y2;
    *writeIterator++ = m_Z;
    *writeIterator++ = colour.r;
    *writeIterator++ = colour.g;
    *writeIterator++ = colour.b;
    *writeIterator++ = colour.a;
    *writeIterator++ = right;
    *writeIterator++ = top;

    *writeIterator++ = x3;
    *writeIterator++ = y3;
    *writeIterator++ = m_Z;
    *writeIterator++ = colour.r;
    *writeIterator++ = colour.g;
    *writeIterator++ = colour.b;
    *writeIterator++ = colour.a;
    *writeIterator++ = right;
    *writeIterator++ = bottom;

    *writeIterator++ = x1;
    *writeIterator++ = y1;
    *writeIterator++ = m_Z;
    *writeIterator++ = colour.r;
    *writeIterator++ = colour.g;
    *writeIterator++ = colour.b;
    *writeIterator++ = colour.a;
    *writeIterator++ = left;
    *writeIterator++ = top;

    *writeIterator++ = x3;
    *writeIterator++ = y3;
    *writeIterator++ = m_Z;
    *writeIterator++ = colour.r;
    *writeIterator++ = colour.g;
    *writeIterator++ = colour.b;
    *writeIterator++ = colour.a;
    *writeIterator++ = right;
    *writeIterator++ = bottom;

    *writeIterator++ = x4;
    *writeIterator++ = y4;
    *writeIterator++ = m_Z;
    *writeIterator++ = colour.r;
    *writeIterator++ = colour.g;
    *writeIterator++ = colour.b;
    *writeIterator++ = colour.a;
    *writeIterator++ = left;
    *writeIterator++ = bottom;

    m_RenderOp.vertexData->vertexCount += 6;

    m_VertexBuffer->unlock();
}



void
Entity::renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation )
{
    if( queueGroupId == Ogre::RENDER_QUEUE_MAIN )
    {
        m_RenderSystem->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
        m_RenderSystem->_setViewMatrix( CameraManager::getSingleton().GetCurrentCamera()->getViewMatrix( true ) );
        m_RenderSystem->_setProjectionMatrix( CameraManager::getSingleton().GetCurrentCamera()->getProjectionMatrixRS() );

        if( m_RenderOp.vertexData->vertexCount != 0 )
        {
            m_SceneManager->_setPass( m_Material->getTechnique( 0 )->getPass( 0 ), true, false );
            m_RenderSystem->_render( m_RenderOp );
        }
    }
}



void
Entity::CreateVertexBuffer()
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
Entity::DestroyVertexBuffer()
{
    delete m_RenderOp.vertexData;
    m_RenderOp.vertexData = 0;
    m_VertexBuffer.setNull();
}



void
Entity::CreateMaterial()
{
    m_Material = Ogre::MaterialManager::getSingleton().create( "Entity", "General" );
    Ogre::Pass* pass = m_Material->getTechnique( 0 )->getPass( 0 );
    pass->setVertexColourTracking( Ogre::TVC_AMBIENT );
    pass->setCullingMode( Ogre::CULL_NONE );
    pass->setDepthCheckEnabled( true );
    pass->setDepthWriteEnabled( true );
    pass->setLightingEnabled( false );
    //pass->setPolygonMode( Ogre::PolygonMode::PM_WIREFRAME );
    pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
    pass->setAlphaRejectFunction( Ogre::CMPF_GREATER );
    pass->setAlphaRejectValue( 0 );
    Ogre::TextureUnitState* tex = pass->createTextureUnitState();
    tex->setTextureName( "system/blank.png" );
    tex->setNumMipmaps( -1 );
    tex->setTextureFiltering( Ogre::TFO_NONE );
}
