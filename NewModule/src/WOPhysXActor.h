#pragma once

#include "WO.h"
#include "Model.h"
#include "PhysicsModule.h"
#include "PxPhysicsAPI.h"

namespace Aftr {
	class NetMessengerClient;
	class PhysicsModule;

	class WOPhysXActor : public WO {
	public:
		WOMacroDeclaration(WOPhysXActor, WO);
		static WOPhysXActor* New(physx::PxRigidActor* pxActor = nullptr,
			const std::string& path = "../mm/models/sphere.dae",
			Vector scale = Vector(1, 1, 1),
			MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
		virtual void onCreate(const std::string& path,
			Vector scale = Vector(1, 1, 1),
			MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
		void setDisplayMatrix(Mat4 matrix);
		physx::PxRigidActor* getActor();

		int index;
	protected:
		WOPhysXActor(physx::PxRigidActor* pxActor);
		physx::PxRigidActor* physxActor;
	};

} // Namespace Aftr