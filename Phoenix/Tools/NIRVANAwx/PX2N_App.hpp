// PX2N_App.hpp

#ifndef PX2N_APP_HPP
#define PX2N_APP_HPP

#include "PX2N_Pre.hpp"
#include "PX2EventHandler.hpp"

namespace NA
{

	class N_Frame;
	class N_PopUpMenuWindow;
	class N_GeneralFrame;
	class PX2::Event;

	class N_App : public wxApp, public PX2::EventHandler
	{
	public:
		N_App();
		virtual ~N_App();

		virtual bool OnInit();
		virtual int OnExit();
		virtual void CleanUp();

		virtual void OnEvent(PX2::Event *event);

	protected:
		N_Frame *CreateMainFrame();

		enum SimuType
		{
			ST_APP,
			ST_SERVER,
			ST_MAX_TYPE
		};
		void SimuApp(SimuType st);
	};

	DECLARE_APP(N_App)
}

#endif