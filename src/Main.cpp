#include <OgreRoot.h>
#include <Overlay/OgreOverlaySystem.h>
#include <OIS.h>

#include "Main.h"
#include "core/CameraManager.h"
#include "core/ConfigCmdManager.h"
#include "core/ConfigFile.h"
#include "core/ConfigVarManager.h"
#include "core/Console.h"
#include "core/DebugDraw.h"
#include "game/EntityManager.h"
#include "core/GameFrameListner.h"
#include "core/InputManager.h"
#include "core/Logger.h"
#include "core/ScriptManager.h"
#include "core/TextManager.h"
#include "core/Timer.h"
#include "core/UiManager.h"



QG_STATE  g_ApplicationState;



int
main(int argc, char *argv[])
{
    Ogre::Root* root;
    Ogre::RenderWindow* window;
    Ogre::SceneManager* scene_manager;

    Ogre::LogManager* log_manager = new Ogre::LogManager();
    log_manager->createLog( "x-gears.log", true, true );
    log_manager->getDefaultLog()->setLogDetail( ( Ogre::LoggingLevel )3 );

    // init root early
    root = new Ogre::Root( "", "" );
#ifndef _DEBUG
        root->loadPlugin( "./RenderSystem_Direct3D9.dll" );
#else
        root->loadPlugin( "./RenderSystem_Direct3D9_d.dll" );
#endif
    root->setRenderSystem( root->getAvailableRenderers()[ 0 ] );
    root->initialise( false );
    Ogre::NameValuePairList misc;
    misc[ "title" ] = QG_VERSION_NAME;
    window = root->createRenderWindow( "QGearsWindow", 1280, 720, false, &misc );
    window->setDeactivateOnFocusChange( false );



    Timer* timer = new Timer();



    // set scene camera and viewport for other moduls
    // create this before initialize particle because some of them use scene to create themself
    scene_manager = root->createSceneManager( Ogre::ST_GENERIC, "Scene" );
    scene_manager->setAmbientLight( Ogre::ColourValue( 1.0, 1.0, 1.0 ) );

    Ogre::OverlaySystem* overlay_system = new Ogre::OverlaySystem();
    scene_manager->addRenderQueueListener( overlay_system );

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation( "./data", "FileSystem", "Game", true );
    Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup( "Game" );




    // init it before console because it may use it
    ConfigVarManager* config_var_manager = new ConfigVarManager();
    ConfigCmdManager* config_cmd_manager = new ConfigCmdManager();



    DebugDraw* debug_draw = new DebugDraw();



    // init before GameFrameListener, but after ConfigCmdManager
    InputManager* input_manager = new InputManager();



    // create This earlier than DisplayFrameListener cause it can fire event there
    CameraManager* camera_manager = new CameraManager();
    TextManager* text_manager = new TextManager();
    UiManager* ui_manager = new UiManager();
    EntityManager* entity_manager = new EntityManager();
    Console* console = new Console();

    // init after game managers because it attach them to script
    ScriptManager* script_manager = new ScriptManager();


    // set base listner for usual game moduls
    GameFrameListener* frame_listener = new GameFrameListener( window );
    root->addFrameListener( frame_listener );



    // execute config
    {
        ConfigFile config;
        config.Execute( "./data/config.cfg" );
    }



    // init ui and run it scripts
    ui_manager->Initialise();



    // run application cycle
    g_ApplicationState = QG_GAME;
    root->startRendering();



    // system modules
    // we must remove this first cause this can fire event to console
    root->removeFrameListener( frame_listener );
    delete frame_listener;
    // destroy before script manager because it removes things from it.
    delete entity_manager;
    delete ui_manager;
    delete text_manager;
    delete script_manager;
    delete console;
    delete camera_manager;
    delete input_manager;
    delete debug_draw;
    delete config_cmd_manager;
    delete config_var_manager;
    delete timer;
    delete root;
    delete log_manager;

    return 0;
}
