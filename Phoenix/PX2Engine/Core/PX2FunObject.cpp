// PX2FunObject.cpp

#include "PX2FunObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// FunParam
//----------------------------------------------------------------------------
FunParam::FunParam() :
Type(FPT_INT)
{
}
//----------------------------------------------------------------------------
FunParam::~FunParam()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FunObject
//----------------------------------------------------------------------------
FunObject::FunObject() :
IsClassCatalogue(false),
mParamType(PT_NONE),
IsStatic(false)
{
}
//----------------------------------------------------------------------------
FunObject::~FunObject()
{
}
//----------------------------------------------------------------------------
void FunObject::Save(XMLNode node)
{
	node.SetAttributeString("Name", Name);
	node.SetAttributeBool("IsClassCatalogue", IsClassCatalogue);
	node.SetAttributeString("ParentClassName", ParentClassName);
	node.SetAttributeBool("IsStatic", IsStatic);
	node.SetAttributeString("ClassName", ClassName);
	node.SetAttributeString("SigletonName", SigletonName);
	node.SetAttributeInt("ParamType", mParamType);

	int numInParam = (int)mInParams.size();
	int numOutParam = (int)mOutParams.size();

	node.SetAttributeInt("NumInParam", numInParam);
	node.SetAttributeInt("NumOutParam", numOutParam);

	XMLNode paramNodeIn = node.NewChild("ParamsIn");
	for (int i = 0; i < numInParam; i++)
	{
		XMLNode paramNode = paramNodeIn.NewChild("param");

		FunParam &funParam = mInParams[i];
		paramNode.SetAttributeString("name", funParam.Name);
		paramNode.SetAttributeInt("type", funParam.Type);
	}

	XMLNode paramNodeOut = node.NewChild("ParamsOut");
	for (int i = 0; i < numOutParam; i++)
	{
		XMLNode paramNode = paramNodeOut.NewChild("param");

		FunParam &funParam = mOutParams[i];
		paramNode.SetAttributeString("name", funParam.Name);
		paramNode.SetAttributeInt("type", funParam.Type);
	}
}
//----------------------------------------------------------------------------
void FunObject::Clear()
{
	mInParams.clear();
	mOutParams.clear();
}
//----------------------------------------------------------------------------
void FunObject::AddInput(const std::string &paramName,
	FunParamType type, const Any &paramValue)
{
	FunParam funParam;
	funParam.Name = paramName;
	funParam.Type = type;
	funParam.Value = paramValue;
	mInParams.push_back(funParam);
}
//----------------------------------------------------------------------------
void FunObject::RemoveInput(const std::string &paramName)
{
	auto it = mInParams.begin();
	for (; it != mInParams.end(); it++)
	{
		if (paramName == it->Name)
		{
			mInParams.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void FunObject::AddOutput(const std::string &paramName, FunParamType type,
	const Any &paramValue)
{
	FunParam funParam;
	funParam.Name = paramName;
	funParam.Type = type;
	funParam.Value = paramValue;
	mOutParams.push_back(funParam);
}
//----------------------------------------------------------------------------
const std::vector<FunParam> &FunObject::GetInParams() const
{
	return mInParams;
}
//----------------------------------------------------------------------------
int FunObject::GetNumInParams() const
{
	return (int)mInParams.size();
}
//----------------------------------------------------------------------------
const FunParam &FunObject::GetInParam(int i) const
{
	return mInParams[i];
}
//----------------------------------------------------------------------------
void FunObject::ClearInParams()
{
	mInParams.clear();
}
//----------------------------------------------------------------------------
const std::vector<FunParam> &FunObject::GetOutParams() const
{
	return mOutParams;
}
//----------------------------------------------------------------------------
int FunObject::GetNumOutParams() const
{
	return (int)mOutParams.size();
}
//----------------------------------------------------------------------------
const FunParam &FunObject::GetOutParam(int i) const
{
	return mOutParams[i];
}
//----------------------------------------------------------------------------
void FunObject::ClearOutParams()
{
	mOutParams.clear();
}
//----------------------------------------------------------------------------
void FunObject::SetParamType(ParamType pt)
{
	mParamType = pt;
}
//----------------------------------------------------------------------------
FunObject::ParamType FunObject::GetParamType() const
{
	return mParamType;
}
//----------------------------------------------------------------------------
FunObject *FunObject::GetAddClass(const std::string &className)
{
	if (IsClassCatalogue && ClassName == className)
		return this;

	std::vector<Pointer0<FunObject> >::iterator it = mChildFunObjectVec_Class.begin();
	for (; it != mChildFunObjectVec_Class.end(); it++)
	{
		FunObject *funObj = (*it)->GetAddClass(className);
		if (funObj)
		{
			return funObj;
		}
	}

	FunObjectPtr funObj = new0 FunObject();
	funObj->IsClassCatalogue = true;
	funObj->ClassName = className;

	AddFunObject(funObj);

	return funObj;
}
//----------------------------------------------------------------------------
FunObject *FunObject::GetFunObject(const std::string &className,
	const std::string &funName)
{
	if (!IsClassCatalogue && ClassName == className && Name == funName)
		return this;

	std::vector<Pointer0<FunObject> >::iterator it = mChildFunObjectVec.begin();
	for (; it != mChildFunObjectVec.end(); it++)
	{
		FunObject *funObj = (*it)->GetFunObject(className, funName);
		if (funObj)
		{
			return funObj;
		}
	}

	std::vector<Pointer0<FunObject> >::iterator it1 = mChildFunObjectVec_Class.begin();
	for (; it1 != mChildFunObjectVec_Class.end(); it1++)
	{
		FunObject *funObj = (*it1)->GetFunObject(className, funName);
		if (funObj)
		{
			return funObj;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void FunObject::AddFunObject(FunObject *funObj)
{
	if (IsHasFunObject(funObj))
		return;

	if (funObj->IsClassCatalogue)
	{
		mChildFunObjectVec_Class.push_back(funObj);
	}
	else
	{
		funObj->ClassName = ClassName;
		mChildFunObjectVec.push_back(funObj);
	}

}
//----------------------------------------------------------------------------
bool FunObject::IsHasFunObject(FunObject *funObj)
{
	for (int i = 0; i<(int)mChildFunObjectVec.size(); i++)
	{
		if (funObj->Name == mChildFunObjectVec[i]->Name)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// FunObjectManager
//----------------------------------------------------------------------------
FunObjectManager::FunObjectManager()
{
}
//----------------------------------------------------------------------------
FunObjectManager::~FunObjectManager()
{
}
//----------------------------------------------------------------------------
bool FunObjectManager::Initlize()
{
	return true;
}
//----------------------------------------------------------------------------
bool FunObjectManager::Terminate()
{
	mObjectFunObject = 0;
	mFunMap.clear();

	return true;
}
//----------------------------------------------------------------------------
void FunObjectManager::SetObjectFunObject(FunObject *funObject)
{
	mObjectFunObject = funObject;
}
//----------------------------------------------------------------------------
FunObject *FunObjectManager::GetObjectFunObject()
{
	return mObjectFunObject;
}
//----------------------------------------------------------------------------
std::map<std::string, FunObjectPtr> FunObjectManager::GetFunMap()
{
	return mFunMap;
}
//----------------------------------------------------------------------------
FunObject *FunObjectManager::GetAddFunMap(const std::string &name)
{
	std::map<std::string, FunObjectPtr>::iterator it = mFunMap.find(name);
	if (it != mFunMap.end())
	{
		return it->second;
	}

	FunObject *funObj = new0 FunObject();
	mFunMap[name] = funObj;

	return funObj;
}
//----------------------------------------------------------------------------