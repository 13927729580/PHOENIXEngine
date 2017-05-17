// PX2EngineClientConnector.hpp

#ifndef PX2ENGINECLIENTCONNECT_HPP
#define PX2ENGINECLIENTCONNECT_HPP

#include "PX2CorePre.hpp"
#include "PX2NetClientConnector.hpp"
#include "PX2EngineNetDefine.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EngineClientConnector : public ClientConnector
	{
	public:
		EngineClientConnector();
		virtual ~EngineClientConnector();

		virtual int Update(float elapsedSeconds);

		void SetAutoConnect(bool autoCnt);
		bool IsAutoConnect() const;
		void SetAutoConnectTime(float seconds);
		float GetAutoConnectTime() const;

		void SetAutoConnectIP(const std::string &ipStr);
		void SetAutoConnectPort(int port);
		const std::string &GetAutoConnectIP() const;
		int GetAutoConnectPort() const;

		bool IsConnected() const;
		void OnConnectedToServer();
		void OnDisConnectedToServer();

		void SendString(const std::string &str);
		void SendPushProject();

		bool IsHasOnConnectCallback(const std::string &callback) const;
		void AddOnConnectCallback(const std::string &callback);
		void RemoveOnConnectCallback(const std::string &callback);

		bool IsHasOnDisConnectCallback(const std::string &callback) const;
		void AddOnDisconnectCallback(const std::string &callback);
		void RemoveOnDisconnectCallback(const std::string &callback);

		bool IsHasOnRecvCallback(const std::string &callback) const;
		void AddOnRecvCallback(const std::string &callback);
		void RemoveOnRecvCallback(const std::string &callback);

	protected:
		int OnStringMsg(const void *pbuffer, int buflen);
		void _SendHeart();

		bool mIsAutoConnect;
		float mAutoConnectTime;
		float mAutoConnectTiming;
		std::string mAutoConnectIP;
		int mAutoConnectPort;

		bool mIsServerConnected;

		float mHeartTiming;

		std::vector<std::string> mOnConnectCallbacks;
		std::vector<std::string> mOnDisconnectCallbacks;
		std::vector<std::string> mOnRecvCallbacks;
	};
	typedef Pointer0<EngineClientConnector> EngineClientConnectorPtr;

}

#endif