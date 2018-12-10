#include "UiSprite9.h"

#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>

#include "Logger.h"



UiSprite9::UiSprite9( const Ogre::String& name ):
    UiWidget( name )
{
    Initialise();
}



UiSprite9::UiSprite9( const Ogre::String& name, const Ogre::String& path_name, UiWidget* parent ):
    UiWidget( name, path_name, parent )
{
    Initialise();
}



UiSprite9::~UiSprite9()
{
}



void
UiSprite9::Initialise()
{
    m_UpperLeft = new UiSprite( "UL", m_Name + ".UL", this );
    AddChild( m_UpperLeft );
    m_UpperCenter = new UiSprite( "UC", m_Name + ".UC", this );
    AddChild( m_UpperCenter );
    m_UpperRight = new UiSprite( "UR", m_Name + ".UR", this );
    AddChild( m_UpperRight );
    m_MiddleLeft = new UiSprite( "ML", m_Name + ".ML", this );
    AddChild( m_MiddleLeft );
    m_MiddleCenter = new UiSprite( "MC", m_Name + ".MC", this );
    AddChild( m_MiddleCenter );
    m_MiddleRight = new UiSprite( "MR", m_Name + ".MR", this );
    AddChild( m_MiddleRight );
    m_BottomLeft = new UiSprite( "BL", m_Name + ".BL", this );
    AddChild( m_BottomLeft );
    m_BottomCenter = new UiSprite( "BC", m_Name + ".BC", this );
    AddChild( m_BottomCenter );
    m_BottomRight = new UiSprite( "BR", m_Name + ".BR", this );
    AddChild( m_BottomRight );
}



void
UiSprite9::Update()
{
    UiWidget::Update();
}



void
UiSprite9::Render()
{
    UiWidget::Render();
}



void
UiSprite9::UpdateTransformation()
{
    UiWidget::UpdateTransformation();
}



void
UiSprite9::SetTexture( const Ogre::String& texture )
{
    m_UpperLeft->SetTexture( texture );
    m_UpperCenter->SetTexture( texture );
    m_UpperRight->SetTexture( texture );
    m_MiddleLeft->SetTexture( texture );
    m_MiddleCenter->SetTexture( texture );
    m_MiddleRight->SetTexture( texture );
    m_BottomLeft->SetTexture( texture );
    m_BottomCenter->SetTexture( texture );
    m_BottomRight->SetTexture( texture );
}



void
UiSprite9::SetUV( const float u1, const float u2, const float u3, const float u4, const float v1, const float v2, const float v3, const float v4 )
{
    m_UpperLeft->SetUV( u1, v1, u2, v1, u1, v2, u2, v2 );
    m_UpperCenter->SetUV( u2, v1, u3, v1, u2, v2, u3, v2 );
    m_UpperRight->SetUV( u3, v1, u4, v1, u3, v2, u4, v2 );
    m_MiddleLeft->SetUV( u1, v2, u2, v2, u1, v3, u2, v3 );
    m_MiddleCenter->SetUV( u2, v2, u3, v2, u2, v3, u3, v3 );
    m_MiddleRight->SetUV( u3, v2, u4, v2, u3, v3, u4, v3 );
    m_BottomLeft->SetUV( u1, v3, u2, v3, u1, v4, u2, v4 );
    m_BottomCenter->SetUV( u2, v3, u3, v3, u2, v4, u3, v4 );
    m_BottomRight->SetUV(  u3, v3, u4, v3, u3, v4, u4, v4 );
}



void
UiSprite9::SetBorderWidth( const float width )
{
    m_UpperLeft->SetX( 0, 0 );
    m_UpperLeft->SetY( 0, 0 );
    m_UpperLeft->SetWidth( 0, width );
    m_UpperLeft->SetHeight( 0, width );

    m_UpperCenter->SetX( 0, width );
    m_UpperCenter->SetY( 0, 0 );
    m_UpperCenter->SetWidth( 100, -width * 2 );
    m_UpperCenter->SetHeight( 0, width );

    m_UpperRight->SetX( 100, -width );
    m_UpperRight->SetY( 0, 0 );
    m_UpperRight->SetWidth( 0, width );
    m_UpperRight->SetHeight( 0, width );

    m_MiddleLeft->SetX( 0, 0 );
    m_MiddleLeft->SetY( 0, width );
    m_MiddleLeft->SetWidth( 0, width );
    m_MiddleLeft->SetHeight( 100, -width * 2 );

    m_MiddleCenter->SetX( 0, width );
    m_MiddleCenter->SetY( 0, width );
    m_MiddleCenter->SetWidth( 100, -width * 2 );
    m_MiddleCenter->SetHeight( 100, -width * 2 );

    m_MiddleRight->SetX( 100, -width );
    m_MiddleRight->SetY( 0, width );
    m_MiddleRight->SetWidth( 0, width );
    m_MiddleRight->SetHeight( 100, -width * 2 );

    m_BottomLeft->SetX( 0, 0 );
    m_BottomLeft->SetY( 100, -width );
    m_BottomLeft->SetWidth( 0, width );
    m_BottomLeft->SetHeight( 0, width );

    m_BottomCenter->SetX( 0, width );
    m_BottomCenter->SetY( 100, -width );
    m_BottomCenter->SetWidth( 100, -width * 2 );
    m_BottomCenter->SetHeight( 0, width );

    m_BottomRight->SetX( 100, -width );
    m_BottomRight->SetY( 100, -width );
    m_BottomRight->SetWidth( 0, width );
    m_BottomRight->SetHeight( 0, width );
}
