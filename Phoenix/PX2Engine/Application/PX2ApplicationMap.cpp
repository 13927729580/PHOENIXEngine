// PX2ApplicationMap.cpp

#include "PX2Application.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2PluginManager.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Application::NewProject(const std::string &pathname,
	const std::string &projName, int so, int width, int height)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	CloseProject();

	TheProject = new0 Project();
	TheProject->SetName(projName);
	TheProject->SetScreenOrientation((Project::ScreenOrientation)so);
	TheProject->SetSize((float)width, (float)height);
	mProjectFilePath = pathname;
	mProjectName = projName;

	PX2_SC_LUA->SetUserTypePointer("PX2_PROJ", "Project", (Project*)TheProject);

	Event *ent = PX2_CREATEEVENTEX(ProjectES, NewProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	Event *entUI = PX2_CREATEEVENTEX(ProjectES, NewUI);
	entUI->SetData<UI*>(TheProject->GetUI());
	PX2_EW.BroadcastingLocalEvent(entUI);
}
//----------------------------------------------------------------------------
void _ReWriteProcessXMLNode(XMLNode node)
{
	std::string writeablePath = PX2_RM.GetWriteablePath();
	const std::string &updateWritePath = PX2_RM.GetDataUpdateWritePath();

	XMLNode childNode = node.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();
		std::string filename = childNode.AttributeToString("filename");
		std::string md5Str = childNode.AttributeToString("md5");
		std::string strData = "Data/";
		std::string subFilename = filename.substr(strData.length(), filename.length() - strData.length());

		if ("directory" == name)
		{
			PX2_RM.CreateFloder(writeablePath + updateWritePath, subFilename);
			_ReWriteProcessXMLNode(childNode);
		}
		else if ("file" == name)
		{
			std::string outPath;
			std::string outFilename;
			StringHelp::SplitFilename(subFilename, outPath, outFilename);

			PX2_RM.CreateFloder(writeablePath + updateWritePath, outPath);

			std::string dstPath = updateWritePath + subFilename;
			PX2_RM.ReWriteFileToWriteablePath(filename, dstPath);
		}

		childNode = node.IterateChild(childNode);
	}
}
//----------------------------------------------------------------------------
void Application::_ProcessReWrite(const std::string &projName)
{
	std::string writeablePath = PX2_RM.GetWriteablePath();
	const std::string &updateWritePath = PX2_RM.GetDataUpdateWritePath();

	std::string writeDataPath = writeablePath + "DataUpdate/";
	std::string projNamePath = projName + "/";

	PX2_RM.CreateFloder(writeDataPath, projNamePath);

	std::string filesPath = "Data/" + projName + "/" + "filelist.xml";
	PX2_RM.ReWriteFileToWriteablePath(filesPath,
		updateWritePath + projName + "/" + "filelist.xml");

	int bufSize = 0;
	char *buf = 0;
	if (PX2_RM.LoadBuffer(filesPath, bufSize, buf))
	{
		XMLData data;
		if (data.LoadBuffer(buf, bufSize))
		{
			XMLNode rootNode = data.GetRootNode();
			_ReWriteProcessXMLNode(rootNode);
		}
	}
}
//----------------------------------------------------------------------------
bool Application::LoadProject(const std::string &name)
{
	if (name.empty())
		return false;

	std::string writeablePath = PX2_RM.GetWriteablePath();
	std::string writeDataPath = writeablePath + "DataUpdate/";
	std::string projWriteDataPath = writeDataPath + name + "/";

	if (mBoostInfo.IsDataReWriteToDataUpdate)
	{
		if (!PX2_RM.IsFileFloderExist(projWriteDataPath))
		{
			PX2_RM.CreateFloder(writeablePath, "DataUpdate/");
			_ProcessReWrite(name);
		}
	}

	std::string writeDataFilelistPath = projWriteDataPath + "filelist.xml";
	if (!PX2_RM.LoadFileTableXML(PX2_RM.GetDataUpdateFileTable(), writeDataFilelistPath))
	{
		PX2_LOG_INFO("LoadFileTableXML %s failed", writeDataFilelistPath.c_str());

		PX2_RM.LoadFileTableXML(PX2_RM.GetDataFiletable(), "Data/" + name + "/filelist.xml");
	}
	else
	{
		PX2_LOG_INFO("LoadFileTableXML %s suc", writeDataFilelistPath.c_str());
	}

	std::string addrPath = "ftp://127.0.0.1/";
	const std::string &resUpdateAddr = PX2_RM.GetResourceUpdateAddr();
	if (!resUpdateAddr.empty())
		addrPath = resUpdateAddr;

	PX2_RM.DoResourceUpdateStuffs(addrPath, name);

	std::string path = "Data/" + name + "/" + name + ".px2proj";
	return PX2_APP.LoadProjectByPath(path);
}
//----------------------------------------------------------------------------
bool Application::LoadProjectByPath(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	CloseProject();
	
	// load

	bool loadResult = false;
	TheProject = new0 Project();
	if (TheProject->Load(pathname))
	{
		PX2_SC_LUA->SetUserTypePointer("PX2_PROJ", "Project", Project::GetSingletonPtr());

		Event *ent = PX2_CREATEEVENTEX(ProjectES, LoadedProject);
		PX2_EW.BroadcastingLocalEvent(ent);

		std::string sceneFilename = TheProject->GetSceneFilename();
		if (!sceneFilename.empty())
		{
			LoadScene(sceneFilename);
		}

		std::string uiFilename = TheProject->GetUIFilename();
		if (!uiFilename.empty())
		{
			LoadUI(uiFilename);
		}

		mProjectFilePath = pathname;
		mProjectName = TheProject->GetName();

		loadResult = true;
	}
	else
	{
		Project::Destory();

		loadResult = false;
	}

	std::string outPath;
	std::string outBaseName;
	std::string outExt;
	StringHelp::SplitFullFilename(pathname, outPath, outBaseName, outExt);

	std::string projName = outBaseName;
	mProjectName = projName;

#if defined (WIN32) || defined (_WIN32)

	std::string projDllFolder = GetProjDataFolderPath(projName);
	std::string projDllFilename = GetDllFileName(projName);
	std::string dllFullpathFilename = projDllFolder + projDllFilename;

	PX2_LOG_INFO("Project dll filename:%s", dllFullpathFilename.c_str());

	if (PX2_RM.IsFileFloderExist(dllFullpathFilename))
	{
		PX2_LOG_INFO("Begin load project dll: %s", dllFullpathFilename.c_str());

		PX2_PLUGINMAN.Load(dllFullpathFilename);

		PX2_LOG_INFO("End load project dll: %s", dllFullpathFilename.c_str());
	}

#elif defined (__LINUX__)
	std::string projDllFolder = GetProjDataFolderPath(projName);
	std::string projDllFilename = GetDllFileName(projName);
	std::string dllFullpathFilename = projDllFolder + projDllFilename;

	PX2_LOG_INFO("Project dll filename:%s", dllFullpathFilename.c_str());

	if (PX2_RM.IsFileFloderExist(dllFullpathFilename))
	{
		PX2_LOG_INFO("Begin load project so: %s", dllFullpathFilename.c_str());

		PX2_PLUGINMAN.Load(dllFullpathFilename);

		PX2_LOG_INFO("End load project so: %s", dllFullpathFilename.c_str());
	}

#elif defined (__ANDROID__)
	//std::string soFilename = "lib" + projName + "SO.so";
	//std::string originSOFilename = "Data/" + soFilename;
	//const std::string writePath = PX2_RM.GetWriteablePath();
	//std::string writePathSOFilename = writePath + soFilename;

	//char *soBuf = 0;
	//int soSize = 0;
	//if (PX2_RM.LoadBuffer(originSOFilename, soSize, soBuf))
	//{
	//	bool ret = FileIO::Save(writePathSOFilename, true, soSize, soBuf);
	//	PX2_LOG_INFO("FileIO::Save ret %s", ret ? "true" : "false");
	//	PX2_LOG_INFO("SoSize:%d", soSize);

	//	PX2_LOG_INFO("Begin load project so: %s", writePathSOFilename.c_str());

	//	PX2_PLUGINMAN.Load(writePathSOFilename);

	//	PX2_LOG_INFO("End load project so: %s", writePathSOFilename.c_str());
	//}
#endif

	std::string callFilenameLua = "Data/" + projName + "/scripts/lua/editor/editor.lua";
	PX2_SC_LUA->CallFileFunction(callFilenameLua.c_str(), "editorstart");

	PX2_LOG_INFO("Loaded Project: %s", projName.c_str());

	return loadResult;
}
//----------------------------------------------------------------------------
std::string Application::GetProjDataFolderPath(const std::string &projName)
{
	std::string folder;

#if defined(_WIN32) || defined(WIN32) 
	folder = "Data/" + projName + "/";
#elif defined (__LINUX__)
	folder = "Data/" + projName + "/";
#else
	folder = PX2_RM.GetWriteablePath();
#endif

	return folder;
}
//----------------------------------------------------------------------------
std::string Application::GetDllFileName(const std::string &projName)
{
	std::string projDllName;

#if defined(_WIN32) || defined(WIN32) 
	std::string tag = "";

#ifdef PX2_USE_OPENGL
	tag += "OpenGL";
#endif

#if defined(_WIN64) || defined(WIN64)
	tag += "64";
#endif

#ifdef _DEBUG
	tag += "D";
#endif

	projDllName = projName + tag + ".dll";

#elif defined(__ANDROID__)
	projDllName = "lib" + projName + ".so";

#elif defined (__LINUX__)

#ifdef PX2_USE_OPENGLES
	projDllName = "libARM" + projName + ".so";
#else
	projDllName = "lib" + projName + ".so";
#endif

#endif

	return projDllName;
}
//----------------------------------------------------------------------------
bool Application::SaveProject()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(mProjectFilePath))
			{
				Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedProject);
				PX2_EW.BroadcastingLocalEvent(ent);

				return true;
			}
		}
		else
		{
			proj->SaveConfig(mProjectFilePath);
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveProjectAs(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(pathname))
			{
				Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedProject);
				PX2_EW.BroadcastingLocalEvent(ent);

				return true;
			}
		}
		else
		{
			proj->SaveConfig(pathname);
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::CloseProject()
{
	PX2_LOGICM.Clear();
	PX2_SELECTM.Clear();
	URDoManager::GetSingleton().Clear();
	URStateManager::GetSingleton().Clear();

	PX2_SS.PlayMusic(0, 0, true, 0.0f);

	Project *oldProj = Project::GetSingletonPtr();
	if (!oldProj) return;

	std::string projName = PX2_PROJ.GetName();
	std::string projDllFolder = GetProjDataFolderPath(projName);
	std::string projDllFilename = GetDllFileName(projName);
	std::string dllFullpathFilename = projDllFolder + projDllFilename;

	Play(Application::PT_NONE);

	CloseScene();
	CloseUI();

	Event *ent = PX2_CREATEEVENTEX(ProjectES, CloseProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	std::string debugTag = "";
#ifdef _DEBUG
	debugTag = "D";
#endif

#if defined (WIN32) || defined (_WIN32)
	if (PX2_RM.IsFileFloderExist(dllFullpathFilename))
	{
		PX2_PLUGINMAN.Unload(dllFullpathFilename);
	}

#endif

	Project::Destory();

	PX2_RM.ClearRes(mProjectFilePath);
	PX2_RM.Clear();
	mProjectFilePath.clear();
}
//----------------------------------------------------------------------------
void Application::NewScene()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	CloseScene();

	Scene *scene = new0 Scene();
	PX2_PROJ.SetScene(scene);
	PX2_PROJ.SetSceneFilename("");
}
//----------------------------------------------------------------------------
bool Application::LoadScene(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	if (!Project::GetSingletonPtr()) return false;

	CloseScene();

	Scene *newscene = DynamicCast<Scene>(PX2_RM.BlockLoad(pathname));
	if (newscene)
	{
		mSceneFilePath = pathname;

		Project::GetSingleton().SetScene(newscene);
		Project::GetSingleton().SetSceneFilename(pathname);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveScene(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	std::string toPath = _CalSavePath(pathname);

	if (_SaveSceneInternal(toPath))
	{
		PX2_PROJ.SetSceneFilename(toPath);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveSceneAs(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	std::string toPath = _CalSavePath(pathname);

	return _SaveSceneInternal(toPath);
}
//----------------------------------------------------------------------------
void Application::CloseScene()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (scene)
	{
		PX2_RM.ClearRes(mSceneFilePath);
		PX2_PROJ.SetScene(0);
		PX2_PROJ.SetSceneFilename("");
		mSceneFilePath.clear();
	}
}
//----------------------------------------------------------------------------
bool Application::LoadUI(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	ObjectPtr uiObj = PX2_RM.BlockLoad(pathname);
	UI *ui0 = DynamicCast<UI>(uiObj);
	if (ui0)
	{
		mUIFilePath = pathname;
		Project::GetSingleton().SetUI(ui0);
	}
	else
	{
		mUIFilePath = pathname;
		UIPtr ui1 = new0 UI();
		Project::GetSingleton().SetUI(ui1);
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::CloseUI()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	EngineUICanvas *uiCanvas = EngineUICanvas::GetSingletonPtr();
	if (uiCanvas)
		uiCanvas->Clear();

	PX2_RM.ClearRes(mUIFilePath);
	PX2_PROJ.SetUI(0);
	mUIFilePath.clear();
}
//----------------------------------------------------------------------------
std::string Application::_CalSavePath(const std::string &pathname)
{
	std::string pathStand = StringHelp::StandardisePath(pathname);
	pathStand = pathStand.substr(0, pathStand.length() - 1);

	std::string toPath = pathStand;
	size_t begin = toPath.find("Data/");
	if (begin != std::string::npos)
	{
		size_t length = toPath.length() - begin;
		toPath = toPath.substr(begin, length);
	}

	return toPath;
}
//----------------------------------------------------------------------------
bool Application::_SaveSceneInternal(const std::string &pathname)
{
	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return false;

	scene->SetResourcePath(pathname);

	OutStream outStream;

	outStream.Insert(scene);

	if (outStream.Save(pathname))
	{
		Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedScene);
		PX2_EW.BroadcastingLocalEvent(ent);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------