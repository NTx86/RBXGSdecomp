#pragma once
#include "reflection/signal.h"
#include "Replicator.h"
#include <RakNetTypes.h>
#include <string>

namespace RBX
{
	namespace Network
	{
		extern const char* sServer;

		class Server : public DescribedCreatable<Server, Peer, &sServer>, public Service
		{
		private:
			int outgoingPort;
			boost::scoped_ptr<worker_thread> pingThread;
			//boost::shared_ptr<Players> players;
			static int lastId;
			static worker_thread::work_result ping(boost::weak_ptr<Server> server, std::string publicIP, int thumbnailId, std::string pingUrl);
		public:
			static Reflection::SignalDesc<Server, void(std::string, boost::shared_ptr<Instance>)> event_IncommingConnection;
		public:
			//void Server(Server const &);
			Server();
			~Server();
			static bool serverIsPresent(Instance const *context, bool testInDatamodel);
			void start(int port, int threadSleepTime);
			void stop(int blockDuration);
			int getClientCount();
			virtual XmlElement *write();
			virtual PluginReceiveResult OnReceive(RakPeerInterface *peer, Packet *packet);
			void setServerManagerPing(std::string pingUrl, std::string publicIP, int thumbnailId);
		protected:
			virtual void onServiceProvider(ServiceProvider const *, ServiceProvider const *);
			virtual bool askAddChild(Instance const *instance) const;
		public:
			//Server &operator=(Server const &);
		private:
			class ClientProxy {};
		};
	}
}