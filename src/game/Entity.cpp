#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

#include "Entity.h"



Entity::Entity( Ogre::SceneNode* node ):
    EntityTile( node ),
    m_CollisionMask( 0 )
{
    SetColour( Ogre::ColourValue( 1, 1, 1, 1 ) );

    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create( "Entity", "General" );
    Ogre::Pass* pass = material->getTechnique( 0 )->getPass( 0 );
    pass->setVertexColourTracking( Ogre::TVC_AMBIENT );
    pass->setCullingMode( Ogre::CULL_NONE );
    pass->setDepthCheckEnabled( true );
    pass->setDepthWriteEnabled( true );
    pass->setLightingEnabled( false );
    pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
    pass->setAlphaRejectFunction( Ogre::CMPF_GREATER );
    pass->setAlphaRejectValue( 0 );
    Ogre::TextureUnitState* tex = pass->createTextureUnitState();
    tex->setTextureName( "system/blank.png" );
    tex->setNumMipmaps( -1 );
    tex->setTextureFiltering( Ogre::TFO_NONE );

    SetMaterial( material );

    UpdateGeometry();
}



Entity::~Entity()
{
}



void
Entity::SetOccupation( const std::vector< Ogre::Vector3 >& occupation )
{
    m_Occupation = occupation;
}



const std::vector< Ogre::Vector3 >&
Entity::GetOccupation() const
{
    return m_Occupation;
}



void
Entity::SetCollisionMask( const int mask )
{
    m_CollisionMask = mask;
}



const int
Entity::GetCollisionMask() const
{
    return m_CollisionMask;
}



void
Entity::SetAction( const Action action )
{
    m_Action = action;
}



const Action
Entity::GetAction() const
{
    return m_Action;
}
