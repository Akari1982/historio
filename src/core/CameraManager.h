#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <OgreCamera.h>
#include <OgreSingleton.h>

#include "Event.h"



class CameraManager : public Ogre::Singleton< CameraManager >
{
public:
    CameraManager();
    virtual ~CameraManager();

    void Input( const Event& event );
    void Update();
    void OnResize();

    Ogre::Camera* GetCurrentCamera();
    Ogre::Viewport* GetCurrentViewport();
    const Ogre::Vector3 ProjectPointToScreen( const Ogre::Vector3& point );

private:

    void InitCommands();

private:
    Ogre::Camera* m_Camera;
    Ogre::Viewport* m_Viewport;

    float m_Z;
};



#endif // CAMERA_MANAGER_H
