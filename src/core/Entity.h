#ifndef ENTITY_H
#define ENTITY_H

#include <OgreString.h>



class Entity : public Ogre::RenderQueueListener
{
public:
    Entity();
    virtual ~Entity();

    void UpdateDebug();

    void SetImage( const Ogre::String& image );
    void UpdateGeometry();

    void renderQueueEnded( Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation );

private:
    Entity();
    void CreateVertexBuffer();
    void DestroyVertexBuffer();
    void CreateMaterial();

protected:
    Ogre::MaterialPtr m_Material;
    Ogre::SceneManager* m_SceneManager;
    Ogre::RenderSystem* m_RenderSystem;
    Ogre::RenderOperation m_RenderOp;
    Ogre::HardwareVertexBufferSharedPtr m_VertexBuffer;
};



#endif // ENTITY_H
