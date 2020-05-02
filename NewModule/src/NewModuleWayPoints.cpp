#include "WOWayPointAbstract.h"
#include "NewModuleWayPoints.h"
#include "GLViewNewModule.h"
#include <vector>
#include <iostream>

namespace Aftr
{

WOWP1* WOWP1::New(const WayPointParametersBase& params, float radius )
{
   WOWP1* ptr = new WOWP1(params, radius);
   ptr->onCreate();
   return ptr;
}

WOWP1::WOWP1(const WayPointParametersBase& params, float radius
             ) : WOWayPointSpherical( params, radius ), IFace( this )
{
}

WOWP1::~WOWP1()
{
}

void WOWP1::onTrigger()
{
   std::cout << "WOWP1 waypoint Triggered!" << std::endl << std::endl;
   float x = rand() % 100;
   if ((int)x % 2 == 0) {
	   x *= -1;
   }
   float y = rand() % 100;
   if ((int)y % 2 == 0) {
	   y *= -1;
   }
   this->setPosition(Vector(x, y, rand() % 50));

}

} //namespace Aftr