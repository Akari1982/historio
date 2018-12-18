#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>
#include <OgreRoot.h>
#include "../core/Logger.h"
#include "MapSector.h"
#include "MapTilesXmlFile.h"



MapSector::MapSector()
{
    for( int i = 0; i < 100; ++i )
    {
        for( int j = 0; j < 100; ++j )
        {
            m_PassMap[ i ][ j ] = 0x0;
        }
    }

    MapTilesXmlFile* tile_file = new MapTilesXmlFile( "data/map_tiles.xml" );
    tile_file->LoadDesc( this );
    delete tile_file;

    CreateVertexBuffers();
    CreateMaterial();

    Ogre::AxisAlignedBox aabb;
    aabb.setInfinite();
    setBoundingBox( aabb );
}



MapSector::~MapSector()
{
    DestroyVertexBuffers();
}



Ogre::Real
MapSector::getSquaredViewDepth( const Ogre::Camera* cam ) const
{
    return 0;
}



Ogre::Real
MapSector::getBoundingRadius() const
{
    return 0;
}



void
MapSector::AddMapTileDesc( const MapTileDesc& desc )
{
    for( size_t i = 0; i < m_MapTileDescs.size(); ++i )
    {
        if( m_MapTileDescs[ i ].name == desc.name )
        {
            m_MapTileDescs[ i ] = desc;
            return;
        }
    }
    m_MapTileDescs.push_back( desc );
}



void
MapSector::Quad( const unsigned int x, const unsigned int y, const float width, const float height, const Ogre::String& name )
{
    if( mRenderOp.vertexData->vertexCount + 6 > m_MaxVertexCount )
    {
        LOG_ERROR( "MapSector: Max number of quads reached. Can't create more than " + Ogre::StringConverter::toString( m_MaxVertexCount / 6 ) + " quads." );
        return;
    }

    float x1 = x - width / 2;
    float y1 = y - height / 2;
    float x2 = x + width / 2;
    float y2 = y - height / 2;
    float x3 = x + width / 2;
    float y3 = y + height / 2;
    float x4 = x - width / 2;
    float y4 = y + height / 2;

    float left = 0.0f;
    float right = 1.0f;
    float top = 0.0f;
    float bottom = 1.0f;
    Ogre::ColourValue colour = Ogre::ColourValue( 1, 1, 1, 0 );

    for( size_t i = 0; i < m_MapTileDescs.size(); ++i )
    {
        if( m_MapTileDescs[ i ].name == name )
        {
            Ogre::Vector4 coord = m_MapTileDescs[ i ].texture_coords;
            left = coord.x;
            right = coord.z;
            top = coord.y;
            bottom = coord.w;
            colour = m_MapTileDescs[ i ].colour;
            if( x < 100 && y < 100 )
            {
                m_PassMap[ x ][ y ] = m_MapTileDescs[ i ].collision_mask;
            }
        }
    }

    float m_Z = 0.0f;

    float* writeIterator = ( float* ) m_VertexBuffer->lock( Ogre::HardwareBuffer::HBL_NORMAL );
    writeIterator += mRenderOp.vertexData->vertexCount * 9;

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

    mRenderOp.vertexData->vertexCount += 6;

    m_VertexBuffer->unlock();
}



const int
MapSector::GetPass( const unsigned int x, const unsigned int y ) const
{
    if( ( x < 100 ) && ( y < 100 ) )
    {
        return m_PassMap[ x ][ y ];
    }
    return -1;
}



void
MapSector::CreateVertexBuffers()
{
    m_MaxVertexCount = 10000 * 6;
    mRenderOp.vertexData = new Ogre::VertexData();
    mRenderOp.vertexData->vertexStart = 0;

    Ogre::VertexDeclaration* vDecl = mRenderOp.vertexData->vertexDeclaration;

    size_t offset = 0;
    vDecl->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE );
    offset += Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT4 );
    vDecl->addElement( 0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

    m_VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer( vDecl->getVertexSize( 0 ), m_MaxVertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY );

    mRenderOp.vertexData->vertexBufferBinding->setBinding( 0, m_VertexBuffer );
    mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    mRenderOp.useIndexes = false;
}



void
MapSector::DestroyVertexBuffers()
{
    delete mRenderOp.vertexData;
    mRenderOp.vertexData = 0;
    m_VertexBuffer.setNull();
    m_MaxVertexCount = 0;
}



void
MapSector::CreateMaterial()
{
    mMaterial = Ogre::MaterialManager::getSingleton().create( "Map", "General" );
    Ogre::Pass* pass = mMaterial->getTechnique( 0 )->getPass( 0 );
    pass->setVertexColourTracking( Ogre::TVC_AMBIENT );
    pass->setCullingMode( Ogre::CULL_NONE );
    pass->setDepthCheckEnabled( true );
    pass->setDepthWriteEnabled( true );
    pass->setLightingEnabled( false );
    pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
    pass->setAlphaRejectFunction( Ogre::CMPF_GREATER );
    pass->setAlphaRejectValue( 0 );
    Ogre::TextureUnitState* tex = pass->createTextureUnitState();
    tex->setTextureName( "Terrain.png" );
    tex->setNumMipmaps( -1 );
    tex->setTextureFiltering( Ogre::TFO_NONE );
}
