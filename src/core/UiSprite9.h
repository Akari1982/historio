#ifndef UI_SPRITE_9_H
#define UI_SPRITE_9_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreRoot.h>

#include "UiWidget.h"



class UiSprite9 : public UiWidget
{
public:
    UiSprite9( const Ogre::String& name );
    UiSprite9( const Ogre::String& name, const Ogre::String& path_name, UiWidget* parent );
    virtual ~UiSprite9();

    void Initialise();
    virtual void Update();
    virtual void Render();
    virtual void UpdateTransformation();

    void SetImage( const Ogre::String& image );
    void UpdateGeometry();

private:
    UiSprite9();
    void CreateVertexBuffer();
    void DestroyVertexBuffer();

private:
    Ogre::MaterialPtr m_Material;
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;

    Ogre::RenderOperation m_RenderOp;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;
};



#endif // UI_SPRITE_9_H
