#include "Server.h"
#include "util/standardout.h"
#include "util/Debug.h"

namespace RBX
{
	namespace Network
	{
		void Server::start(int port, int threadSleepTime)
		{
			SocketDescriptor socketDescriptor(port, "");
			if (!this->rakPeer.get()->Startup(32, threadSleepTime, &socketDescriptor, 1))
			{
				std::string error("Failed to start network server");
				throw std::runtime_error(error);
			}
			this->outgoingPort = port;

			StandardOut::singleton()->print(MESSAGE_INFO, "Starting network server on port %d", port);

			unsigned NumberOfAddresses = this->rakPeer->GetNumberOfAddresses();

			StandardOut::singleton()->print(MESSAGE_INFO, "IP addresses:");

			unsigned int index = 0;
			if(NumberOfAddresses > 0)
			{
				do
				{
					StandardOut::singleton()->print(MESSAGE_INFO, "%s", this->rakPeer->GetLocalIP(index));
					index++;
				}
				while (index < NumberOfAddresses);
			}
			Peer::updateNetworkSimulator();
		}

		void Server::stop(int blockDuration)
		{
			if(this->rakPeer.get()->IsActive())
			{
				this->rakPeer.get()->Shutdown(blockDuration);
			}
			Instance::removeAllChildren();
		}

		bool Server::askAddChild(Instance const *instance) const
		{
			return dynamic_cast<ClientProxy*>(const_cast<Instance*>(instance)) != NULL;
		}
	}
}