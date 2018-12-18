#ifndef ENTITY_MOVABLE_H
#define ENTITY_MOVABLE_H

#include "Entity.h"



class EntityMovable : public Entity
{
public:
    EntityMovable( Ogre::SceneNode* node );
    virtual ~EntityMovable();

    virtual void Update();
    virtual void UpdateDebug();

    void SetMovePath( std::vector< Ogre::Vector3 >& move_path );
    const std::vector< Ogre::Vector3 >& GetMovePath() const;
    void SetPosition( const Ogre::Vector3& pos );

    Ogre::Vector3 GetMoveNextPosition() const;
    Ogre::Vector3 GetMoveEndPosition() const;

private:
    std::vector< Ogre::Vector3 > m_MovePath;
};



#endif // ENTITY_MOVABLE_H
