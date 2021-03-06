#ifndef UI_SPRITE_H
#define UI_SPRITE_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreRoot.h>

#include "UiWidget.h"



class UiSprite : public UiWidget
{
public:
    UiSprite( const Ogre::String& name );
    UiSprite( const Ogre::String& name, const Ogre::String& path_name, UiWidget* parent );
    virtual ~UiSprite();

    void Initialise();
    virtual void Update();
    virtual void Render();
    virtual void UpdateTransformation();

    void SetTexture( const Ogre::String& texture );
    void SetUV( const float u1, const float v1, const float u2, const float v2, const float u3, const float v3, const float u4, const float v4 );
    void SetVertexShader( const Ogre::String& shader );
    void SetFragmentShader( const Ogre::String& shader );
    void UpdateGeometry();

private:
    UiSprite();
    void CreateVertexBuffer();
    void DestroyVertexBuffer();

private:
    Ogre::MaterialPtr                   m_Material;
    Ogre::SceneManager*                 m_SceneManager;
    Ogre::RenderSystem*                 m_RenderSystem;

    float m_U1;
    float m_V1;
    float m_U2;
    float m_V2;
    float m_U3;
    float m_V3;
    float m_U4;
    float m_V4;

    Ogre::RenderOperation               m_RenderOp;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;
};



#endif // UI_SPRITE_H
