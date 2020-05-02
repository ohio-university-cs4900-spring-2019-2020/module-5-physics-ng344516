#include "GLViewNewModule.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "AftrGLRendererBase.h"

//If we want to use way points, we need to include this.
#include "NewModuleWayPoints.h"
#include "WOPhysXActor.h"
//#include "Rocket.h"
#include "PhysicsModule.h"
#include <WOFTGLString.h>
#include <MGLFTGLString.h>


using namespace Aftr;
using namespace physx;

GLViewNewModule* GLViewNewModule::New( const std::vector< std::string >& args )
{
   GLViewNewModule* glv = new GLViewNewModule( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}

void GLViewNewModule::init(float gScalar, Vector gravityNormVec, std::string confFileName, const PHYSICS_ENGINE_TYPE& physEType) {
	GLView::init(gScalar, gravityNormVec, confFileName, physEType);
}

GLViewNewModule::GLViewNewModule( const std::vector< std::string >& args ) : GLView( args )
{
   //Initialize any member variables that need to be used inside of LoadMap() here.
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewNewModule::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewNewModule::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewNewModule::onCreate()
{
   //GLViewNewModule::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->physicsEngine = PhysicsModule::New();
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1

   std::string port = ManagerEnvironmentConfiguration::getVariableValue("NetServerListenPort");
   std::cout << "Listening on port " << port << std::endl;
   std::cout << "Loading background noise...\n";
   this->gameSounds = SoundModule::init();
   if (port != "12683") {
	   this->client = NetMessengerClient::New("127.0.0.1", "12683");
	   this->gameSounds->play_sound_2D("../mm/sounds/bensound-jazzyfrenchy.ogg", true, false, true);
   }
   else {
	   this->client = NetMessengerClient::New("127.0.0.1", "12684");
	   this->gameSounds->play_sound_2D("../mm/sounds/bensound-goinghigher.ogg", true, false, true);
   }
   this->gameSounds->get_sound_2D().at(0)->setVolume(0.3f);
   PxMaterial* gMaterial = this->physicsEngine->getPhysics()->createMaterial(.5f, .5f, .6f);
   PxMaterial* gMaterial2 = this->physicsEngine->getPhysics()->createMaterial(.5f, .5f, .6f);
   PxRigidStatic* groundPlane = PxCreatePlane(*this->physicsEngine->getPhysics(), PxPlane(PxVec3(0, 0, 1), 0), *gMaterial);
   this->physicsEngine->getScene()->addActor(*groundPlane);

   

}


GLViewNewModule::~GLViewNewModule()
{
   //Implicitly calls GLView::~GLView()
	
}


void GLViewNewModule::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.
   if (!this->gameSounds->get_sound_3D().empty()) {
	   this->gameSounds->get_sound_engine()->setListenerPosition(
		   this->gameSounds->to_vec3df(this->cam->getPosition()),
		   this->gameSounds->to_vec3df(this->cam->getLookDirection()),
		   irrklang::vec3df(0, 0, 0),
		   this->gameSounds->to_vec3df(this->cam->getNormalDirection())
	   );
   }
   this->physicsEngine->simulate(this->client, this->balls);
   /*if (numRockets > 0) {
	   for (int i = 0; i < numRockets; i++) {
		   rockets[i]->updatePosition();
	   }
   }*/
}


void GLViewNewModule::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewNewModule::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewNewModule::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewNewModule::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewNewModule::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );
   if( key.keysym.sym == SDLK_0 )
      this->setNumPhysicsStepsPerRender( 1 );

   if (key.keysym.sym == SDLK_1) 
	{
	   this->gameSounds->play_sound_2D("../mm/sounds/bensound-jazzyfrenchy.ogg", true, false, true);
	   this->gameSounds->get_sound_2D().at(0)->setVolume(0.5f);
	   std::cout << "2D music start\n";
	}
   if( key.keysym.sym == SDLK_2 )
   {
	   this->gameSounds->stop_sound_2D();
	   std::cout << "2D music stop\n";
   }
   if (key.keysym.sym == SDLK_3)
   {
	   this->gameSounds->play_sound_3D("../mm/sounds/explosion.wav", Vector(100, 0, 3), true, false, true);
	   this->gameSounds->get_sound_3D().back()->setVolume(20.0f);
	   this->gameSounds->get_sound_3D().back()->setMinDistance(5.0f);
	   std::cout << "3D music start\n";
   }
   if (key.keysym.sym == SDLK_4)
   {
	   this->gameSounds->stop_sound_3D();
	   std::cout << "3D music stop\n";
   }
   if (key.keysym.sym == SDLK_v)
   {
	   // Spawns rocket in front of camera
	   spawnRocket();
	   
   }
   if (key.keysym.sym == SDLK_f)
   {
	   spawnRocket();
   }
   if (key.keysym.sym == SDLK_g)
   {
	   std::cout << "Physics test\n";
	   spawnBall(spawnInFrontOfPlayer(3.0f));
   }
   if (key.keysym.sym == SDLK_c)
   {
	   std::cout << "Camera location: " << cam->getPosition() << std::endl;
	   std::cout << "Camera look dir: " << cam->getLookDirection() << std::endl;
	   std::cout << "Camera norm dir: " << cam->getNormalDirection() << std::endl;
   }
   if (key.keysym.sym == SDLK_r)
   {
	   cam->setCameraNormalDirection(Vector(0, 0, 1));
	   cam->setCameraLookDirection(Vector(1, 0, 0));
	   cam->setPosition(Vector(15, 15, 10));
   }
}


void GLViewNewModule::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}


void Aftr::GLViewNewModule::loadMap()
{
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->cam->setPosition( 15,15,10 );

   std::string shinyRedPlasticCube( ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl" );
   std::string wheeledCar( ManagerEnvironmentConfiguration::getSMM() + "/models/rcx_treads.wrl" );
   std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
   std::string human( ManagerEnvironmentConfiguration::getSMM() + "/models/human_chest.wrl" );
   
   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
	skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust2+6.jpg" );

   float ga = 0.1f; //Global Ambient Light level for this module
   ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
   WOLight* light = WOLight::New();
   light->isDirectionalLight( true );
   light->setPosition( Vector( 0, 0, 100 ) );
   //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
   //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
   light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
   light->setLabel( "Light" );
   worldLst->push_back( light );

   //Create the SkyBox
   WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
   wo->setPosition( Vector( 0,0,0 ) );
   wo->setLabel( "Sky Box" );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   worldLst->push_back( wo );

   ////Create the infinite grass plane (the floor)
   wo = WO::New( grass, Vector( 1, 1, 1 ), MESH_SHADING_TYPE::mstFLAT );
   wo->setPosition( Vector( 0, 0, 0 ) );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
   grassSkin.getMultiTextureSet().at( 0 )->setTextureRepeats( 20.0f );
   grassSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
   grassSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
   grassSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiney" it is)
   grassSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
   wo->setLabel( "Grass" );
   worldLst->push_back( wo );

   ////Create the infinite grass plane that uses the Open Dynamics Engine (ODE)
   //wo = WOStatic::New( grass, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //((WOStatic*)wo)->setODEPrimType( ODE_PRIM_TYPE::PLANE );
   //wo->setPosition( Vector(0,0,0) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0).getMultiTextureSet().at(0)->setTextureRepeats( 5.0f );
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   ////Create the infinite grass plane that uses NVIDIAPhysX(the floor)
   //wo = WONVStaticPlane::New( grass, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //wo->setPosition( Vector(0,0,0) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0).getMultiTextureSet().at(0)->setTextureRepeats( 5.0f );
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   ////Create the infinite grass plane (the floor)
   //wo = WONVPhysX::New( shinyRedPlasticCube, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //wo->setPosition( Vector(0,0,50.0f) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   //wo = WONVPhysX::New( shinyRedPlasticCube, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //wo->setPosition( Vector(0,0.5f,75.0f) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   //wo = WONVDynSphere::New( ManagerEnvironmentConfiguration::getVariableValue("sharedmultimediapath") + "/models/sphereRp5.wrl", Vector(1.0f, 1.0f, 1.0f), mstSMOOTH );
   //wo->setPosition( 0,0,100.0f );
   //wo->setLabel( "Sphere" );
   //this->worldLst->push_back( wo );

   //wo = WOHumanCal3DPaladin::New( Vector( .5, 1, 1 ), 100 );
   //((WOHumanCal3DPaladin*)wo)->rayIsDrawn = false; //hide the "leg ray"
   //((WOHumanCal3DPaladin*)wo)->isVisible = false; //hide the Bounding Shell
   //wo->setPosition( Vector(20,20,20) );
   //wo->setLabel( "Paladin" );
   //worldLst->push_back( wo );
   //actorLst->push_back( wo );
   //netLst->push_back( wo );
   //this->setActor( wo );
   //
   //wo = WOHumanCyborg::New( Vector( .5, 1.25, 1 ), 100 );
   //wo->setPosition( Vector(20,10,20) );
   //wo->isVisible = false; //hide the WOHuman's bounding box
   //((WOHuman*)wo)->rayIsDrawn = false; //show the 'leg' ray
   //wo->setLabel( "Human Cyborg" );
   //worldLst->push_back( wo );
   //actorLst->push_back( wo ); //Push the WOHuman as an actor
   //netLst->push_back( wo );
   //this->setActor( wo ); //Start module where human is the actor

   ////Create and insert the WOWheeledVehicle
   //std::vector< std::string > wheels;
   //std::string wheelStr( "../../../shared/mm/models/WOCar1970sBeaterTire.wrl" );
   //wheels.push_back( wheelStr );
   //wheels.push_back( wheelStr );
   //wheels.push_back( wheelStr );
   //wheels.push_back( wheelStr );
   //wo = WOCar1970sBeater::New( "../../../shared/mm/models/WOCar1970sBeater.wrl", wheels );
   //wo->setPosition( Vector( 5, -15, 20 ) );
   //wo->setLabel( "Car 1970s Beater" );
   //((WOODE*)wo)->mass = 200;
   //worldLst->push_back( wo );
   //actorLst->push_back( wo );
   //this->setActor( wo );
   //netLst->push_back( wo );
   
   createNewModuleWayPoints();
}


void GLViewNewModule::createNewModuleWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWP1::New( params, 3 );
   wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( wayPt );
   wayPt = WOWP1::New(params, 3);
   wayPt->setPosition(Vector(100, 0, 3));
   worldLst->push_back(wayPt);
}

void GLViewNewModule::spawnRocket() {
	Vector pos = cam->getPosition();
	Vector dir = cam->getLookDirection();
	float dist = 10;
	WO* wo = WO::New("../mm/models/sphere.dae");
	wo->setPosition(Vector(pos.x + (dist * dir.x), pos.y + (dist * dir.y), pos.z + (dist * dir.z)));
	wo->setLabel("rocket");
	float angle = sqrt((dir.x*dir.x) + (dir.y*dir.y)) / dir.z;
	worldLst->push_back(wo);
	std::cout << "Loading rocket model with:\n"
		<< "Rocket location: " << wo->getPosition() << std::endl
		<< "Rocket look dir: " << wo->getLookDirection() << std::endl
		<< "Rocket norm dir: " << wo->getNormalDirection() << std::endl;
	this->client->sendNetMsgSynchronousTCP(NetModule(wo->getPosition()));
}

void GLViewNewModule::spawnFromNet(Vector position) {
	std::cout << "Spawning rocket sent over net\n";
	WO* wo = WO::New("../mm/models/sphere.dae");
	wo->setPosition(position);
	wo->setLabel("ball");
	worldLst->push_back(wo);
	std::cout << "Added asset from net\n";
}

void GLViewNewModule::spawnBall(Vector spawnLoc) {
	PxTransform trans = PxTransform(PxVec3(spawnLoc.x, spawnLoc.y, spawnLoc.z));
	PxShape* shape = this->physicsEngine->physics->createShape(PxSphereGeometry(0.2f), *this->physicsEngine->physics->createMaterial(.5f, .3f, .2f));
	PxRigidDynamic* actor = PxCreateDynamic(*this->physicsEngine->physics, trans, *shape, 5.0f);
	WOPhysXActor* wo = WOPhysXActor::New(actor, "../mm/models/sphere.dae", Vector(0.1f, 0.1f, 0.1f));
	wo->getActor()->userData = wo;
	wo->setPosition(spawnLoc);
	worldLst->push_back(wo);
	std::cout << "At position " << wo->getPosition() << std::endl;
	this->physicsEngine->scene->addActor(*wo->getActor());
	this->client->sendNetMsgSynchronousTCP(NetModule(wo->getPosition()));
	this->balls.insert(std::pair(wo, numBalls));
	numBalls++;
}

Vector GLViewNewModule::spawnInFrontOfPlayer(float dist) {
	Vector position = this->cam->getPosition();
	Vector direction = this->cam->getLookDirection();
	return Vector(position.x + (dist * direction.x), position.y + (dist * direction.y), position.z + (dist * direction.z));
}
