#ifndef UI_SPRITE_9_H
#define UI_SPRITE_9_H

#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>
#include <OgreRoot.h>

#include "UiSprite.h"
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

    void SetTexture( const Ogre::String& texture );
    void SetUV( const float u1, const float u2, const float u3, const float u4, const float v1, const float v2, const float v3, const float v4 );
    void SetBorderWidth( const float width );

private:
    UiSprite9();

private:
    UiSprite* m_UpperLeft;
    UiSprite* m_UpperCenter;
    UiSprite* m_UpperRight;
    UiSprite* m_MiddleLeft;
    UiSprite* m_MiddleCenter;
    UiSprite* m_MiddleRight;
    UiSprite* m_BottomLeft;
    UiSprite* m_BottomCenter;
    UiSprite* m_BottomRight;
};



#endif // UI_SPRITE_9_H
