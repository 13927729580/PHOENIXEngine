// AppPlay.cpp

#include "AppPlay.hpp"
using namespace appplay;

#if defined (__ANDROID__)
#include <android/log.h>
#endif

#define APPPLAY_PX2
#ifdef APPPLAY_PX2
#include "PX2Log.hpp"
#include "PX2App.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2IMEDispatcher.hpp"
#include "PX2InputManager.hpp"
#include "PX2PlatformSDK.hpp"
#include "PX2Bluetooth.hpp"
#include "PX2VoiceSDK.hpp"
using namespace PX2;
#endif

//----------------------------------------------------------------------------
bool NativeCall::msIsIntlized = false;
//----------------------------------------------------------------------------
bool NativeCall::Initlize(int width, int height)
{

#ifdef APPPLAY_PX2
	if (!IsInitlized())
	{
#if defined (__ANDROID__)
		__android_log_print(ANDROID_LOG_INFO, "appplay.lib", "begin - AppBase::msAppInitlizeFun.");
#endif
		
		PX2::AppBase::msAppInitlizeFun();
		
#if defined (__ANDROID__)
		__android_log_print(ANDROID_LOG_INFO, "appplay.lib", "end - AppBase::msAppInitlizeFun.");
#endif
		
		PX2::AppBase::msApplication->Initlize();
		PX2::AppBase::msApplication->OnSize(width, height);
		PX2::AppBase::msApplication->WillEnterForeground(true);
		
		msIsIntlized = true;
	}
	else
	{
		PX2::AppBase::msApplication->OnSize(width, height);
		PX2::AppBase::msApplication->WillEnterForeground(false);
	}
#endif

	return true;
}
//----------------------------------------------------------------------------
bool NativeCall::IsInitlized()
{
	return msIsIntlized;
}
//----------------------------------------------------------------------------
bool NativeCall::Ternamate()
{
#ifdef APPPLAY_PX2
	if (PX2::AppBase::IsInitlized())
		PX2::AppBase::msAppTernamateFun();
#endif

	return true;
}
//----------------------------------------------------------------------------
bool NativeCall::WillEnterForeground()
{
	/* i do not use it*/
	return true;
}
//----------------------------------------------------------------------------
bool NativeCall::DidEnterBackground()
{

#ifdef APPPLAY_PX2
	if (PX2::AppBase::IsInitlized())
		PX2::AppBase::msApplication->DidEnterBackground();
#endif

	return true;
}
//----------------------------------------------------------------------------
void NativeCall::OnOdle()
{
#ifdef APPPLAY_PX2
	if (PX2::AppBase::IsInitlized())
		PX2::AppBase::msApplication->OnIdle();
#endif
}
//----------------------------------------------------------------------------
void NativeCall::OnTouchPressed(int num, int ids[], float xs[], float ys[])
{

#ifdef APPPLAY_PX2
	InputEventListener *listener = InputManager::GetSingleton().GetDefaultListener();
	listener->TouchPressed(num, ids, xs, ys);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::OnTouchReleased(int num, int ids[], float xs[], float ys[])
{

#ifdef APPPLAY_PX2
	InputEventListener *listener = InputManager::GetSingleton().GetDefaultListener();
	listener->TouchReleased(num, ids, xs, ys);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::OnTouchMoved(int num, int ids[], float xs[], float ys[])
{

#ifdef APPPLAY_PX2
	InputEventListener *listener = InputManager::GetSingleton().GetDefaultListener();
	listener->TouchMoved(num, ids, xs, ys);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::OnTouchCancelled(int num, int ids[], float xs[], float ys[])
{

#ifdef APPPLAY_PX2
	InputEventListener *listener = InputManager::GetSingleton().GetDefaultListener();
	listener->TouchCancelled(num, ids, xs, ys);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::SetResourcePath(std::string path)
{

#ifdef APPPLAY_PX2
	PX2::ResourceManager::SetResourcePath(path);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::SetDataServerUpdateType(std::string type)
{

#ifdef APPPLAY_PX2
	PX2::ResourceManager::SetDataUpdateServerType(type);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::SetWriteablePath(std::string path)
{
    
#ifdef APPPLAY_PX2
	PX2::ResourceManager::SetWriteablePath(path);
#endif
    
}
//----------------------------------------------------------------------------
void NativeCall::SetDeviceIDStr(std::string deviceIDStr)
{
#ifdef APPPLAY_PX2
#endif
}
//----------------------------------------------------------------------------
void NativeCall::Text_InsertText(const char *text, int length)
{

#ifdef APPPLAY_PX2
	IMEDispatcher::GetSingleton().DispathInsertText(text, length);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::Text_DeleteBackward()
{

#ifdef APPPLAY_PX2
	IMEDispatcher::GetSingleton().DispathDeleteBackward();
#endif

}
//----------------------------------------------------------------------------
const char *NativeCall::Text_GetInitContent()
{
	const char * pszText = 0;

#ifdef APPPLAY_PX2
	pszText = IMEDispatcher::GetSingleton().GetContentText();
#endif

	return pszText;
}
//----------------------------------------------------------------------------
void NativeCall::SetPlatformSDK(std::string str)
{ 

#ifdef APPPLAY_PX2
	PX2_PFSDK.SetPlatformSDK(str);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::OnLoginSuc(int apiID, std::string loginUin,
	std::string sessionID, std::string nickname)
{

#ifdef APPPLAY_PX2
	PX2_PFSDK.OnLoginSuc(apiID, loginUin, sessionID, nickname);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::OnLoginCancel()
{

#ifdef APPPLAY_PX2
	PX2_PFSDK.OnLoginCancel();
#endif

}
//----------------------------------------------------------------------------
void NativeCall::OnLoginFailed()
{

#ifdef APPPLAY_PX2
	PX2_PFSDK.OnLoginFailed();
#endif

}
//----------------------------------------------------------------------------
void NativeCall::OnGuestOfficialSuc()
{

#ifdef APPPLAY_PX2
	PX2_PFSDK.OnGuestOfficialSuc();
#endif

}
//----------------------------------------------------------------------------
void NativeCall::BluetoothDiscoveryNewDevice(std::string deviceStr)
{

#ifdef APPPLAY_PX2
	PX2_BLUETOOTH.OnDisocveryNewDevice(deviceStr);
#endif

}
//----------------------------------------------------------------------------
void NativeCall::BluetoothDiscoveryFinished()
{

#ifdef APPPLAY_PX2
	PX2_BLUETOOTH.OnDiscoveryFinished();
#endif

}
//----------------------------------------------------------------------------
void NativeCall::BluetoothConnected()
{
	
#ifdef APPPLAY_PX2
	PX2_BLUETOOTH.OnConnected();
#endif

}
//----------------------------------------------------------------------------
void NativeCall::BluetoothDisConnected()
{
	
#ifdef APPPLAY_PX2
	PX2_BLUETOOTH.OnDisConnected();
#endif

}
//----------------------------------------------------------------------------
void NativeCall::BluetoothConnectFailed()
{
#ifdef APPPLAY_PX2
	PX2_BLUETOOTH.OnConnectFailed();
#endif
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void NativeCall::SetVoiceSDK(std::string strSDK)
{
#if defined (__ANDROID__)
	__android_log_print(ANDROID_LOG_INFO, "appplay.lib", "SetVoiceSDK");
#endif
	
#ifdef APPPLAY_PX2
	PX2_VOICESDK.SetVoiceSDK(strSDK);
#endif
}
//----------------------------------------------------------------------------
void NativeCall::OnVoiceRecognizeResults(std::string strRet, std::string strJSON)
{
#if defined (__ANDROID__)
		__android_log_print(ANDROID_LOG_INFO, "appplay.lib", "OnVoiceRecognizeResults");
#endif
	
#ifdef APPPLAY_PX2
	PX2_VOICESDK.OnVoiceRecognizeResults(strRet, strJSON);
#endif
}
//----------------------------------------------------------------------------
