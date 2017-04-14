// PX2GeneralServer.hpp

#ifndef PX2SIMPLESERVER_HPP
#define PX2SIMPLESERVER_HPP

#include "PX2NetPre.hpp"
#include "PX2NetServer.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	const int GeneralServerMsgID = 2;

	enum GeneralServerCallType
	{
		GSCT_ON_CONNECT,
		GSCT_ON_DISCONNECT,
		GSCT_ON_RECVEDMSG,
		GSCT_MAX_TYPE
	};

	struct PX2_ENGINE_ITEM GeneralServerCallData
	{
		GeneralServerCallData();

		GeneralServerCallType TheCallType;
		int ClientID;
		std::string strData;
	};
	typedef void (*GeneralServerCallback) (GeneralServerCallData data);

	class PX2_ENGINE_ITEM GeneralServer : public Server, public Singleton < GeneralServer >
	{
	public:
		GeneralServer(PX2::Server::ServerType serverType, int port,
			int numMaxConnects, int numMaxMsgHandlers);
		virtual ~GeneralServer();

		void SendString(int clientid, const std::string &str);
		void AddCallback(GeneralServerCallback callBack);

	protected:
		virtual int OnConnect(unsigned int clientid);
		virtual int OnDisconnect(unsigned int clientid);
		int OnString(unsigned int clientid, const void *pbuffer, int buflen);

		std::vector<GeneralServerCallback> mCallbacks;
	};

#define  PX2_SIMPLE_SERVER GeneralServer::GetSingleton()
	typedef Pointer0<GeneralServer> GeneralServerPtr;

}

#endif