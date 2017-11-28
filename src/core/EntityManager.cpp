#include "EntityManager.h"
#include "EntityManagerCommands.h"

#include "ConfigVar.h"
#include "Logger.h"



template<>EntityManager *Ogre::Singleton< EntityManager >::msSingleton = NULL;



EntityManager::EntityManager()
{
    LOG_TRIVIAL( "EntityManager created." );

    InitCmd();

    m_Map = new Map();
    Entity* entity = new Entity();
    m_Entity.push_back( entity );
}



EntityManager::~EntityManager()
{
    for( unsigned int i = 0; i < m_Entity.size(); ++i )
    {
        delete m_Entity[ i ];
    }
    m_Entity.clear();

    LOG_TRIVIAL( "EntityManager destroyed." );
}



void
EntityManager::Input( const Event& event )
{
}



void
EntityManager::Update()
{
    UpdateDebug();
}



void
EntityManager::UpdateDebug()
{
    for( size_t i = 0; i < m_Entity.size(); ++i )
    {
        m_Entity[ i ]->UpdateDebug();
    }
}



void
EntityManager::OnResize()
{
}
