#pragma once

#include "NetMsg.h"
#include "../../../include/aftr/Vector.h"
#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr {

	class NetModule : public NetMsg {
	public:
		NetMsgMacroDeclaration(NetModule);
		NetModule(const Vector position = (0.0, 0.0, 0.0));
		virtual ~NetModule();
		virtual bool toStream(NetMessengerStreamBuffer& os) const;
		virtual bool fromStream(NetMessengerStreamBuffer& is);
		virtual void onMessageArrived();
		virtual std::string toString() const;
		void setPosition(Vector position);
	protected:
		Vector position;
	};

} // Namespace Aftr

#endif
