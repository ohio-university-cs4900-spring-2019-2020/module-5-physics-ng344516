#include "NetModule.h"
#ifdef AFTR_CONFIG_USE_BOOST

#include <iostream>
#include <sstream>
#include <string>
#include "AftrManagers.h"
#include "WO.h"
#include "GLView.h"
#include "WorldContainer.h"
#include "GLViewNewModule.h"


using namespace Aftr;

NetMsgMacroDefinition(NetModule);

NetModule::NetModule(const Vector position) {
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}

NetModule::~NetModule() {

}

bool NetModule::toStream(NetMessengerStreamBuffer& os) const {
	os << this->position.x;
	os << this->position.y;
	os << this->position.z;
	return true;
}

bool NetModule::fromStream(NetMessengerStreamBuffer& is) {
	is >> this->position.x;
	is >> this->position.y;
	is >> this->position.z;
	return true;
}

void NetModule::onMessageArrived() {
	((GLViewNewModule*)ManagerGLView::getGLView())->spawnBall(this->position);
	std::cout << this->toString() << std::endl;
}

std::string NetModule::toString() const {
	std::stringstream ss;
	ss << NetMsg::toString();
	ss << "Payload: { Position: { x: " << this->position.x
		<< ", y: " << this->position.y
		<< ", z: " << this->position.z << " }\n";
	return ss.str();
}

void NetModule::setPosition(Vector position) {
	this->position = position;
}

#endif
