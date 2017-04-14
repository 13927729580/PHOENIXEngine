// PX2LuaPlusContext.cpp

#include "PX2LuaPlusContext.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Assert.hpp"
#include "PX2Log.hpp"
#include "PX2LuaStackBackup.hpp"
#include "PX2LuaScriptController.hpp"
#include "LuaPlus.h"
#include "PX2System.hpp"
#include "PX2TimerManager.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "tolua++.h"
using namespace PX2;
using namespace LuaPlus;

//----------------------------------------------------------------------------
class SleepTimer : public Timer
{
public:
	SleepTimer()
	{
		StartState = 0;
		RefNumCallState = 0;
		RefNumSelf = 0;
	}
	~SleepTimer()
	{

	}

	lua_State *StartState;
	int RefNumCallState;
	int RefNumSelf;

	virtual bool OnTimer(float appTime)
	{
		lua_getref(StartState, RefNumCallState);
		lua_unref(StartState, RefNumCallState);

		lua_getref(StartState, RefNumSelf);
		lua_setglobal(StartState, "self");

		lua_pcall(StartState, 0, 0, 0);

		lua_unref(StartState, RefNumSelf);

		return true;
	}
};
//----------------------------------------------------------------------------
int sleep(lua_State *state)
{
	lua_settop(state, 2);
	int refState = lua_ref(state, LUA_REGISTRYINDEX);
	float timVal = luaL_checknumber(state, 1);

	SleepTimer *timer = TimerManager::GetSingleton().AddTimer<SleepTimer>(
		0.0f, timVal);
	timer->RefNumCallState = refState;

	lua_getglobal(state, "_StartLuaState");
	timer->StartState = (lua_State *)lua_touserdata(state, -1);
	lua_pop(state, 1);

	lua_getglobal(timer->StartState, "self");
	int refthis = lua_ref(state, LUA_REGISTRYINDEX);
	timer->RefNumSelf = refthis;

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
LuaPlusContext *LuaPlusContext::mSingletonLuaPlusContext = 0;
//----------------------------------------------------------------------------
LuaPlusContext::LuaPlusContext() :
ScriptContext(ScriptContext::CT_LUA),
mLuaPlusState(0)
{
	mSingletonLuaPlusContext = this;

	mLuaPlusState = LuaPlus::LuaState::Create(true);

	mLuaPlusState->OpenLibs();
	lua_State *state = (lua_State*)mLuaPlusState;
	lua_pushlightuserdata(state, state);
	lua_setglobal(state, "_StartLuaState");

	mLuaPlusState->GetGlobals().RegisterDirect("CallFile", (*this), 
		&LuaPlusContext::CallFile1);
	mLuaPlusState->GetGlobals().RegisterDirect("CallString", (*this), 
		&LuaPlusContext::CallString1);
	mLuaPlusState->GetGlobals().RegisterDirect("RegistEventFunction", (*this),
		&LuaPlusContext::RegistEventFunction);

	lua_pushcfunction(state, sleep);
	lua_setglobal(state, "sleep");

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
LuaPlusContext::~LuaPlusContext()
{
	ClearEventFunctions();

	LuaPlus::LuaState::Destroy(mLuaPlusState);
}
//----------------------------------------------------------------------------
LuaPlus::LuaState *LuaPlusContext::GetLuaPlusState()
{
	return mLuaPlusState;
}
//----------------------------------------------------------------------------
void *LuaPlusContext::GetLuaState()
{
	return mLuaPlusState->GetCState();
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallString1(const char *str)
{
	return CallString(str);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFile1(const char *filename)
{
	return CallFile(filename);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallString(const std::string &str)
{
	int result = 0;

	if (str.length() <= 1 || str[0] != '=')
	{
		result = mLuaPlusState->DoString(str.c_str());
		if (result != 0)
		{
			OnError(result);
			return false;
		}
	}
	// If the string starts with '=', wrap the statement in the print() function.
	else
	{
		std::string buffer("print(");
		buffer += (str.c_str() + 1);
		buffer += ")";
		result = mLuaPlusState->DoString(buffer.c_str());
		if (result != 0)
		{
			OnError(result);
			return false;
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFile(const std::string &filename)
{
	bool ret = false;

	int bufferSize = 0;
	char *buffer = 0;
	if (ResourceManager::GetSingleton().LoadBuffer(filename, bufferSize,
		buffer))
	{
		ret = CallBuffer(buffer, (unsigned long)bufferSize, filename);
	}

	return ret;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallBuffer(const char *buffer, unsigned long size, 
	const std::string &name)
{
	int result = mLuaPlusState->DoBuffer(buffer, size, name.c_str());
	if (result != 0)
	{
		OnError(result);
	}

	return true;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFileFunction(const std::string &filename,
	const std::string &funName)
{
	if (!filename.empty())
	{
		if (!CallFile(filename))
			return false;
	}

	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;
		
		try
		{
			call << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFunction(const std::string &funName,
	Object *obj0, int paramData1)
{
	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;

		try
		{
			LuaObject objdata0;
			objdata0.AssignLightUserdata(mLuaPlusState, obj0);

			LuaObject objData1;
			objData1.AssignInteger(mLuaPlusState, paramData1);

			call << objdata0 << objData1 << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LuaPlusContext::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	LuaStackBackup stackbackup((lua_State*)mLuaPlusState);
	tolua_pushusertype((lua_State*)mLuaPlusState, ptr, className.c_str());
	lua_setglobal((lua_State*)mLuaPlusState, luaName.c_str());
}
//----------------------------------------------------------------------------
void LuaPlusContext::SetUserFunction(const std::string &funName,
	const std::string &returnClassTypeName,
	ScriptGlobalFun globalFun)
{
	PX2_UNUSED(returnClassTypeName);

	mLuaPlusState->GetGlobals().RegisterDirect(funName.c_str(), globalFun);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallObjectFuntionValist(const std::string &funName,
	Object *paramObj, const std::string &format, va_list valist)
{
	lua_State *state = (lua_State*)mLuaPlusState;

	lua_getglobal(state, "this");
	lua_getglobal(state, paramObj->GetName().c_str());
	lua_setglobal(state, "this");

	const char *pfmt = format.c_str();
	int count = 0;
	static const char *args[] = { "arg0", "arg1", "arg2", "arg3" };
	while (pfmt[count])
	{
		if (*pfmt == 'i')
		{
			int value = va_arg(valist, int);
			lua_pushnumber(state, value);
		}
		else if (*pfmt == 'f')
		{
			float value = (float)(va_arg(valist, double));
			lua_pushnumber(state, value);
		}
		else if (*pfmt == 's')
		{
			char *str = va_arg(valist, char *);
			lua_pushstring(state, str);
		}
		else
		{
			assertion(false, "");
		}
		lua_setglobal(state, args[count++]);
	}

	{
		CallString(funName);
	}

	lua_setglobal(state, "this");

	return true;
}
//----------------------------------------------------------------------------
void LuaPlusContext::ClearEventFunctions()
{
	mEventFunObjects.clear();
}
//----------------------------------------------------------------------------
bool LuaPlusContext::RegistEventFunction(const char *entName,
	LuaPlus::LuaObject callFunObject)
{
	std::string entNameStr(entName);
	if (!entNameStr.empty())
	{
		mEventFunObjects[entNameStr].push_back(callFunObject);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LuaPlusContext::OnEvent(Event *ent)
{
	const std::string &entTypeStr = ent->GetEventTypeStr();
	const std::string &dataStr0 = ent->GetDataStr0();
	const std::string &dataStr1 = ent->GetDataStr1();
	const std::string &dataStr2 = ent->GetDataStr2();

	auto it = mEventFunObjects.begin();
	for (; it != mEventFunObjects.end(); it++)
	{
		const std::string &entName = it->first;
		if (entName == entTypeStr)
		{
			std::vector<LuaPlus::LuaObject> &vecs = it->second;
			for (int i = 0; i < (int)vecs.size(); i++)
			{
				LuaPlus::LuaObject &luaObj = vecs[i];
				if (!luaObj.IsNil())
				{
					LuaCall call = luaObj;

					if (!dataStr0.empty())
					{
						LuaObject luaObj0;
						luaObj0.AssignString(mLuaPlusState, dataStr0.c_str(), dataStr0.length());
						call << luaObj0;
					}
					if (!dataStr1.empty())
					{
						LuaObject luaObj1;
						luaObj1.AssignString(mLuaPlusState, dataStr1.c_str(), dataStr1.length());
						call << luaObj1;
					}
					if (!dataStr2.empty())
					{
						LuaObject luaObj2;
						luaObj2.AssignString(mLuaPlusState, dataStr2.c_str(), dataStr2.length());
						call << luaObj2;
					}

					call << LuaRun();
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void LuaPlusContext::OnError(int errorNum)
{
	PX2_UNUSED(errorNum);

	LuaPlus::LuaStackObject stackObj(mLuaPlusState, -1);
	const char* errStr = stackObj.GetString();
	if (errStr)
	{
		mLastError = errStr;
		ClearStack();
	}
	else
	{
		mLastError = "Unknown Lua parse error";
	}

	PX2_LOG_ERROR("LuaError:%s", mLastError.c_str());
	assertion(false, "LuaError:%s", mLastError.c_str());
}
//----------------------------------------------------------------------------
void LuaPlusContext::ClearStack(void)
{
	mLuaPlusState->SetTop(0);
}
//----------------------------------------------------------------------------