// PX2LogicManager.cpp

#include "PX2LogicManager.hpp"
#include "PX2Float3.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2XMLData.hpp"
#include "PX2FunObject.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2LogicES.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2LuaPlusContext.hpp"
#include "BlueBlockPlugin.hpp"
#include "PX2PluginManager.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
LogicManager::LogicManager() :
mBlueBlockPlugin(0)
{
	mPlatformType = PT_EDITOR;

	mBlueBlockPlugin = new BlueBlockPlugin();
}
//----------------------------------------------------------------------------
LogicManager::~LogicManager()
{
	PX2_PLUGINMAN.UninstallPlugin(mBlueBlockPlugin);
}
//----------------------------------------------------------------------------
bool LogicManager::Initlize()
{
	PX2_LOGICM.AddPkgInfo("Data/engine/pkg/pkg.xml");

	_InitCtrls();
	_InitFuns();

	PX2_PLUGINMAN.InstallPlugin(mBlueBlockPlugin);

	return true;
}
//----------------------------------------------------------------------------
void LogicManager::Clear()
{
	mSelectObject = 0;
}
//----------------------------------------------------------------------------
void LogicManager::SetSelectLogicObject(Object *object)
{
	if (mSelectObject == object)
		return;

	mSelectObject = object;

	Event *ent = PX2_CREATEEVENTEX(LogicES, SetSelectLogicObject);
	ent->SetData<Object*>((Object*)mSelectObject);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
Object *LogicManager::GetSelectLogicObject()
{
	return mSelectObject;
}
//----------------------------------------------------------------------------
void LogicManager::SetCurGeneratedScript(const std::string &str)
{
	mCurGeneratedScript = str;

	Event *ent = PX2_CREATEEVENTEX(LogicES, RefreshGeneratedScript);
	ent->SetDataStr0(mCurGeneratedScript);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
const std::string &LogicManager::GetCurGeneratedScript() const
{
	return mCurGeneratedScript;
}
//----------------------------------------------------------------------------
void LogicManager::SetPlatformType(PlatformType pt)
{
	mPlatformType = pt;
}
//----------------------------------------------------------------------------
LogicManager::PlatformType LogicManager::GetPlatformType() const
{
	return mPlatformType;
}
//----------------------------------------------------------------------------
void LogicManager::SetCurLogicObject(Object *obj)
{
	mCurLogicObject = obj;
}
//----------------------------------------------------------------------------
Object *LogicManager::GetCurLogicObject() const
{
	return mCurLogicObject;
}
//----------------------------------------------------------------------------
void LogicManager::_InitCtrls()
{
	BeginAddFunObj("Program");
	AddOutput("Start", FPT_LINK);
	AddOutput("Update", FPT_LINK);
	AddOutput("FixUpdate", FPT_LINK);
	EndAddFun_Ctrl("Ctrl");

	BeginAddFunObj("If");
	AddInput("If", FPT_BOOL);
	AddOutput("IfDo", FPT_LINK);
	EndAddFun_Ctrl("Ctrl");

	BeginAddFunObj("IfElse");
	AddInput("If", FPT_BOOL);
	AddOutput("IfDo", FPT_LINK);
	AddOutput("ElseDo", FPT_LINK);
	EndAddFun_Ctrl("Ctrl");

	BeginAddFunObj("While");
	AddInput("While", FPT_BOOL);
	AddOutput("WhileDo", FPT_LINK);
	EndAddFun_Ctrl("Ctrl");

	BeginAddFunObj("Coroutine");
	AddOutput("CorDo", FPT_LINK);
	EndAddFun_Ctrl("Ctrl");

	BeginAddFunObj("sleep");
	AddInput("seconds", FPT_FLOAT);
	EndAddFun_General("Ctrl");
}
//----------------------------------------------------------------------------
void LogicManager::_InitFuns()
{
	BeginAddFunObj("FunctionStart");
	EndAddFun_General("GlobalFuns");
}
//----------------------------------------------------------------------------
FunParamType GetParamType(const std::string &typeStr,
	Any &getAnyVal, const std::string &valStr)
{
	// FPT_NONE,
	// FPT_INT,
	// FPT_FLOAT,
	// FPT_FLOAT3,
	// FPT_APOINT,
	// FPT_AVECTOR,
	// FPT_BOOL,
	// FPT_STRING,
	// FPT_POINTER,
	// FPT_POINTER_THIS,
	// FPT_POINTER_THIS_STATIC,
	// FPT_MAX_TYPE

	FunParamType fpt = FPT_NONE;
	Any anyVal;

	if ("void" == typeStr)
	{
		fpt = FPT_NONE;
	}
	else if ("char" == typeStr)
	{
		fpt = FPT_CHAR;
		int iVal = 0;
		if (!valStr.empty())	
			iVal = StringHelp::StringToInt(valStr);
		anyVal = (char)iVal;
	}
	else if ("int" == typeStr)
	{
		fpt = FPT_INT;
		int iVal = 0;
		if (!valStr.empty())
			iVal = StringHelp::StringToInt(valStr);
		anyVal = iVal;
	}
	else if ("float" == typeStr || "double" == typeStr)
	{
		fpt = FPT_FLOAT;
		float fVal = 0.0f;
		if (!valStr.empty())
			fVal = StringHelp::StringToFloat(valStr);
		anyVal = fVal;
	}
	else if ("Float3" == typeStr)
	{
		fpt = FPT_FLOAT3;
		anyVal = Float3::ZERO;
	}
	else if ("APoint" == typeStr)
	{
		fpt = FPT_APOINT;
		anyVal = APoint::ORIGIN;
	}
	else if ("AVector" == typeStr)
	{
		fpt = FPT_AVECTOR;
		anyVal = AVector::ZERO;
	}
	else if ("bool" == typeStr)
	{
		fpt = FPT_BOOL;
		bool bVal = valStr=="true"? true : false;
		anyVal = bVal;
	}
	else if ("std::string" == typeStr || "string" == typeStr)
	{
		fpt = FPT_STRING;
		std::string strValUse;
		if (!valStr.empty())
		{
			if ('"' == valStr[0])
				strValUse = valStr.substr(1, valStr.length() - 2);
			else
				strValUse = valStr;
		}
		anyVal = strValUse;
	}

	getAnyVal = anyVal;

	return fpt;
}
//----------------------------------------------------------------------------
bool LogicManager::AddPkgInfo(const std::string &filename)
{
	int bufSize = 0;
	char *buf = 0;
	if (PX2_RM.LoadBuffer(filename, bufSize, buf))
	{
		XMLData data;
		if (data.LoadBuffer(buf, bufSize))
		{
			XMLNode enumsNode = data.GetNodeByPath("enums");

			XMLNode classesNode = data.GetNodeByPath("classes");
			XMLNode classNode = classesNode.IterateChild();
			while (!classNode.IsNull())
			{
				std::string className = classNode.AttributeToString("name");
				std::string parentName = classNode.AttributeToString("parentname");
				std::string singletonName = classNode.AttributeToString("singletonname");

				FunObjectPtr funObjectClass = new0 FunObject();
				mClassFunsMap[className] = funObjectClass;
				funObjectClass->CateName = "Func";
				funObjectClass->SigletonName = singletonName;
				funObjectClass->IsClassCatalogue = true;
				funObjectClass->ClassName = className;
				funObjectClass->ParentClassName = parentName;

				XMLNode functionNode = classNode.IterateChild();
				while (!functionNode.IsNull())
				{
					std::string strFunName = functionNode.AttributeToString("name");
					std::string strFunType = functionNode.AttributeToString("funtype");
					bool is_operator = functionNode.AttributeToBool("is_operator");
					bool is_purevirtual = functionNode.AttributeToBool("is_purevirtual");
					bool is_virtual = functionNode.AttributeToBool("is_virtual");
					bool is_inline = functionNode.AttributeToBool("is_inline");
					bool is_const = functionNode.AttributeToBool("is_const");

					bool is_class_constructor = ("class_constructor" == strFunType);
					bool is_class_destructor = ("class_destructor" == strFunType);
					bool is_class_member = ("class_member" == strFunType);
					bool is_static = ("class_static" == strFunType);

					if (is_class_member || is_static)
					{
						FunObjectPtr funObj = new0 FunObject;
						funObj->CateName = "Func";
						funObj->SigletonName = singletonName;
						funObj->ClassName = className;
						funObj->Name = strFunName;
						funObjectClass->AddFunObject(funObj);

						if (is_class_member)
						{
							funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
						}
						else if (is_static)
						{
							funObj->AddInput("static", FPT_POINTER_THIS_STATIC, (Object*)0);
						}

						XMLNode paramNode = functionNode.IterateChild();
						while (!paramNode.IsNull())
						{
							std::string typestr = paramNode.AttributeToString("type_str");

							bool is_return = paramNode.AttributeToBool("is_retrun");
							bool is_returnvoid = (std::string("void") == typestr);
							std::string valuename_str = paramNode.AttributeToString("valuename_str");
							std::string defaultStr = paramNode.AttributeToString("defaultvalue_Str");
							bool is_pointer = paramNode.AttributeToBool("is_pointer");
							bool is_ref = paramNode.AttributeToBool("is_ref");

							Any anyData;
							FunParamType fpt = GetParamType(typestr, anyData, defaultStr);

							if (!is_return)
							{
								if (is_pointer)
								{
									funObj->AddInput(valuename_str, FPT_POINTER, (Object*)0);
								}
								else
								{
									funObj->AddInput(valuename_str, fpt, anyData);
								}
							}
							else
							{
								if (!is_returnvoid)
								{
									if (is_pointer)
									{
										funObj->AddOutput("out_val", FPT_POINTER, (Object*)0);
									}
									else
									{
										funObj->AddOutput("out_val", fpt, anyData);
									}
								}
							} 

							paramNode = paramNode.IterateChild(paramNode);
						}
					}

					functionNode = functionNode.IterateChild(functionNode);
				}

				classNode = classNode.IterateChild(classNode);
			}
		}
	}

	auto it = mClassFunsMap.begin();
	for (; it != mClassFunsMap.end(); it++)
	{
		std::string funObjName = it->first;
		FunObject *funObj = it->second;

		if (funObj->IsClassCatalogue)
		{
			const std::string className = funObj->ClassName;
			const std::string &parentClassName = funObj->ParentClassName;

			if ("Object" == className)
			{
				FunObjectManager::GetSingleton().SetObjectFunObject(funObj);
			}

			if (!parentClassName.empty())
			{
				auto itParent = mClassFunsMap.find(parentClassName);
				if (itParent != mClassFunsMap.end())
				{
					(*itParent).second->AddFunObject(funObj);
				}
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------
void LogicManager::BeginAddFunObj(const std::string &funName)
{
	mCurAddFunObj = new0 FunObject();

	mCurAddFunObj->ClearInParams();
	mCurAddFunObj->ClearOutParams();
	mCurAddFunObj->Name = funName;
}
//----------------------------------------------------------------------------
void LogicManager::BeginAddClassFunObj(const std::string &className,
	const std::string &funName, bool isStatic, const std::string &singltonName,
	const std::string &tag)
{
	mCurAddFunObj = new0 FunObject();

	mCurAddFunObj->ClearInParams();
	mCurAddFunObj->ClearOutParams();
	mCurAddFunObj->ClassName = className;
	mCurAddFunObj->Name = funName;
	mCurAddFunObj->IsStatic = isStatic;
	mCurAddFunObj->SigletonName = singltonName;
	mCurAddFunObj->Tag = tag;
}
//----------------------------------------------------------------------------
void LogicManager::BeginAddParam(const std::string &paramName, 
	FunObject::ParamType pt)
{
	mCurAddFunObj = new0 FunObject();

	mCurAddFunObj->ClearInParams();
	mCurAddFunObj->ClearOutParams();
	mCurAddFunObj->Name = paramName;
	mCurAddFunObj->SetParamType(pt);
}
//----------------------------------------------------------------------------
void LogicManager::BeginAddEvent(const std::string &className, 
	const std::string &entName)
{
	mCurAddFunObj = new0 FunObject();

	mCurAddFunObj->ClearInParams();
	mCurAddFunObj->ClearOutParams();
	mCurAddFunObj->Name = entName;
	mCurAddFunObj->ClassName = className;
}
//----------------------------------------------------------------------------
void LogicManager::AddInput(const std::string &name,
	FunParamType funParamType)
{
	if (FPT_CHAR == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, (int)0);
	}
	else if (FPT_INT == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, 0);
	}
	else if (FPT_FLOAT == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, 0.0f);
	}
	else if (FPT_APOINT == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, APoint::ORIGIN);
	}
	else if (FPT_AVECTOR == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, AVector::ZERO);
	}
	else if (FPT_BOOL == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, false);
	}
	else if (FPT_STRING == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, std::string(""));
	}
	else if (FPT_POINTER == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS_STATIC == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, (Object*)0);
	}
	else if (FPT_LINK == funParamType)
	{
		mCurAddFunObj->AddInput(name, funParamType, (Object*)0);
	}
}
//----------------------------------------------------------------------------
void LogicManager::AddInputChar(const std::string &name, char val)
{
	mCurAddFunObj->AddInput(name, FPT_CHAR, (int)val);
}
//----------------------------------------------------------------------------
void LogicManager::AddInputInt(const std::string &name, int val)
{
	mCurAddFunObj->AddInput(name, FPT_INT, val);
}
//----------------------------------------------------------------------------
void LogicManager::AddInputFloat(const std::string &name, float val)
{
	mCurAddFunObj->AddInput(name, FPT_FLOAT, val);
}
//----------------------------------------------------------------------------
void LogicManager::AddInputBool(const std::string &name, bool val)
{
	mCurAddFunObj->AddInput(name, FPT_BOOL, val);
}
//----------------------------------------------------------------------------
void LogicManager::AddInputString(const std::string &name, const std::string &val)
{
	mCurAddFunObj->AddInput(name, FPT_STRING, val);
}
//----------------------------------------------------------------------------
void LogicManager::AddInputObj(const std::string &name)
{
	mCurAddFunObj->AddInput(name, FPT_POINTER, (Object*)0);
}
//----------------------------------------------------------------------------
void LogicManager::AddOutput(const std::string &name,
	FunParamType funParamType)
{
	if (FPT_CHAR == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, (int)0);
	}
	else if (FPT_INT == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, 0);
	}
	else if (FPT_FLOAT == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, 0.0f);
	}
	else if (FPT_APOINT == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, APoint::ORIGIN);
	}
	else if (FPT_AVECTOR == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, AVector::ZERO);
	}
	else if (FPT_BOOL == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, true);
	}
	else if (FPT_STRING == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, std::string(""));
	}
	else if (FPT_POINTER == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, (Object*)0);
	}
	else if (FPT_POINTER_THIS_STATIC == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, (Object*)0);
	}
	else if (FPT_LINK == funParamType)
	{
		mCurAddFunObj->AddOutput(name, funParamType, (Object*)0);
	}
}
//----------------------------------------------------------------------------
void LogicManager::EndAddFun_Ctrl(const std::string &catalogue)
{
	_AddCtrl(catalogue, mCurAddFunObj);
}
//----------------------------------------------------------------------------
void LogicManager::EndAddFun_Param(const std::string &catalogue)
{
	_AddParam(catalogue, mCurAddFunObj);
}
//----------------------------------------------------------------------------
void LogicManager::RemoveParam(const std::string &paramName)
{
	auto it = mParams.begin();
	for (; it != mParams.end(); it++)
	{
		std::vector<std::string> &paramsVec = it->second;
		auto it1 = paramsVec.begin();
		for (; it1 != paramsVec.end();)
		{
			if (paramName == *it1)
			{
				it1 = paramsVec.erase(it1);
			}
			else
			{
				it1++;
			}
		}
	}

	auto itt = mParamObjects.find(paramName);
	if (itt != mParamObjects.end())
	{
		mParamObjects.erase(itt);
	}
}
//----------------------------------------------------------------------------
void LogicManager::EndAddFun_Operator(const std::string &catalogue)
{
	_AddOPerator(catalogue, mCurAddFunObj);
}
//----------------------------------------------------------------------------
void LogicManager::EndAddFun_General(const std::string &catalogue)
{
	_AddGenFun(catalogue, mCurAddFunObj);
}
//----------------------------------------------------------------------------
void LogicManager::EndAddFun_Event(const std::string &catalogue)
{
	_AddEvent(catalogue, mCurAddFunObj);
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetCurAddFunObject()
{
	return mCurAddFunObj;
}
//----------------------------------------------------------------------------
void LogicManager::_AddGenFun(const std::string &catalogue, FunObject *funObj)
{
	mGenFunObjects[funObj->Name] = funObj;
	mGeneralFunctions[catalogue].push_back(funObj->Name);
	funObj->CateName = catalogue;
}
//----------------------------------------------------------------------------
void LogicManager::_AddCtrl(const std::string &catalogue,
	FunObject *funObj)
{
	mCtrls[catalogue].push_back(funObj->Name);
	mCtrlObjects[funObj->Name] = funObj;
	funObj->CateName = catalogue;
}
//----------------------------------------------------------------------------
void LogicManager::_AddParam(const std::string &catalogue, FunObject *funObj)
{
	mParams[catalogue].push_back(funObj->Name);
	mParamObjects[funObj->Name] = funObj;
	funObj->CateName = catalogue;
}
//----------------------------------------------------------------------------
void LogicManager::_AddOPerator(const std::string &catalogue, FunObject *funObj)
{
	mOperators[catalogue].push_back(funObj->Name);
	mOperatorsObjects[funObj->Name] = funObj;
	funObj->CateName = catalogue;
}
//----------------------------------------------------------------------------
void LogicManager::_AddEvent(const std::string &catalogue, FunObject *funObj)
{
	mEvents[catalogue].push_back(funObj->Name);
	mEventObjects[funObj->Name] = funObj;
	funObj->CateName = catalogue;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetClassFunObject(const std::string &className)
{
	auto it = mClassFunsMap.find(className);
	if (it != mClassFunsMap.end())
	{
		FunObject *classFunObj = it->second;
		if (classFunObj)
		{
			return classFunObj;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetClassFunObject(const std::string &className, 
	const std::string &funName)
{
	FunObject *funObj = 0;

	auto it = mClassFunsMap.find(className);
	if (it != mClassFunsMap.end())
	{
		FunObject *classFunObj = it->second;
		if (classFunObj)
		{
			funObj = classFunObj->GetFunObject(className, funName);
		}
	}

	if (funObj) return funObj;

	funObj = GetEvent(funName);
	if (funObj) return funObj;

	return funObj;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetFunObject(const std::string &name)
{
	FunObject *funObj = GetEvent(name);
	if (funObj) return funObj;

	funObj = GetGF(name);
	if (funObj) return funObj;

	funObj = GetCtrl(name);
	if (funObj) return funObj;

	funObj = GetParam(name);
	if (funObj) return funObj;

	funObj = GetOperator(name);
	if (funObj) return funObj;

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetEvent(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it =
		mEventObjects.find(name);
	if (it != mEventObjects.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetGF(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it =
		mGenFunObjects.find(name);
	if (it != mGenFunObjects.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetCtrl(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it =
		mCtrlObjects.find(name);
	if (it != mCtrlObjects.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
const std::map<std::string, std::vector<std::string> > &LogicManager::
GetParams() const
{
	return mParams;
}
//----------------------------------------------------------------------------
std::map<std::string, std::vector<std::string> > &LogicManager::GetParams()
{
	return mParams;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetParam(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it =
		mParamObjects.find(name);
	if (it != mParamObjects.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
FunObject *LogicManager::GetOperator(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it =
		mOperatorsObjects.find(name);
	if (it != mOperatorsObjects.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------'