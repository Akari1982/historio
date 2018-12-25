#ifndef ENTITY_MOVABLE_H
#define ENTITY_MOVABLE_H

#include "Entity.h"



class EntityMovable : public Entity
{
public:
    EntityMovable( Ogre::SceneNode* node );
    virtual ~EntityMovable();

    void SetMovePath( std::vector< Ogre::Vector3 >& move_path );
    const std::vector< Ogre::Vector3 >& GetMovePath() const;
    Ogre::Vector3 GetMoveNext() const;
    void SetMoveEnd( const Ogre::Vector3& end );
    const Ogre::Vector3& GetMoveEnd() const;

private:
    std::vector< Ogre::Vector3 > m_MovePath;
    Ogre::Vector3 m_MoveEnd;
};



#endif // ENTITY_MOVABLE_H
