// GeneralPlugin.hpp

#ifndef BLUEBLOCKPLUGIN_HPP
#define BLUEBLOCKPLUGIN_HPP

#include "PX2Plugin.hpp"
#include "General_Pre.hpp"

namespace PX2
{

	class GENERAL_DLL_ITEM GeneralPlugin : public Plugin
	{
	public:
		GeneralPlugin();
		virtual ~GeneralPlugin();

		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();

	private:
		static Plugin *sPlugin;
	};

}
#endif