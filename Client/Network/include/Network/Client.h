#pragma once
#include "winsock2.h"
#include "reflection/signal.h"
#include "v8xml/XmlElement.h"
#include "Replicator.h"
#include <RakNetTypes.h>
#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX
{
	namespace Network
	{
		extern const char* sClient;

		class Client : public DescribedCreatable<Client, Peer, &sClient>, public Service, public Listener<ServiceProvider, Closing>
		{
		private:
			SystemAddress serverId;
		public:
			static Reflection::SignalDesc<Client, void(std::string, boost::shared_ptr<Instance>)> event_ConnectionAccepted;
			static Reflection::SignalDesc<Client, void(std::string, int)> event_ConnectionFailed;
			static Reflection::SignalDesc<Client, void(std::string)> event_ConnectionRejected;

		public:
			//Client(const Client&);
			Client();
			~Client();
			void connect(std::string, int, int, int);
			void disconnect(int blockDuration);
			virtual PluginReceiveResult OnReceive(RakPeerInterface*, Packet*);
			virtual XmlElement* write();
		protected:
			virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
			virtual void onEvent(const ServiceProvider* source, Closing event);
		public:
			//Client& operator=(const Client&);

			static bool clientIsPresent(const Instance* context, bool testInDatamodel);
		};
	}
}
