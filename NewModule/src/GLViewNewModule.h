#pragma once

#include "GLView.h"
#include "SoundModule.h"
#include "NetModule.h"
#include "PhysicsModule.h"
#include "PxPhysicsAPI.h"
#include <NetMessengerClient.h>

#include <WOFTGLString.h>

namespace Aftr
{
   class Camera;
   class PhysicsModule;

/**
   \class GLViewNewModule
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewNewModule : public GLView
{
public:
   static GLViewNewModule* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewNewModule();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createNewModuleWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );

   virtual void init(float gScalar, Vector gravityNormVec, std::string confFileName, const PHYSICS_ENGINE_TYPE& physEType);
   void spawnRocket();
   void spawnFromNet(Vector position);

   void spawnBall(Vector spawnLoc);
   Vector spawnInFrontOfPlayer(float dist);

   std::map<WOPhysXActor*, int> balls;
   int numBalls = 0;

   //typedef Rocket* RocketPtr;
  // RocketPtr* rockets = new RocketPtr[10];
  // int numRockets = 0;
   //int rocketIndex = 0;

protected:
   GLViewNewModule( const std::vector< std::string >& args );
   virtual void onCreate();
   SoundModule* gameSounds;
   NetMessengerClient* client;
   PhysicsModule* physicsEngine;
};

/** \} */

} //namespace Aftr
