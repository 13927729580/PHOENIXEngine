// PX2PropertyGrid.hpp

#ifndef PX2PROPERTYGRID_HPP
#define PX2PROPERTYGRID_HPP

#include "PX2N_Pre.hpp"
#include "PX2PropertyPage.hpp"

namespace NA
{

	class PropertyGrid : public wxWindow, public PX2::EventHandler
	{
	public:
		PropertyGrid (wxWindow *parent);
		virtual ~PropertyGrid ();

		DECLARE_DYNAMIC_CLASS(PropertyGrid)

		void Clear ();

		// ����ҳ
		PropertyPage *AddPropertyPage (std::string name);
		int GetPropertyPageNum () { return (int)mPages.size(); }
		PropertyPage *GetPropertyPage (std::string name);
		PropertyPage *GetPropertyPage (wxPGProperty *prop);
		void RemovePropertyPage (std::string name);
		void RemoveAll ();
		void ActivatePropertyPage (std::string name);
		void ActivatePropertyPage (PropertyPage *page);
		PropertyPage *GetActivatePage ();

		void OnSize(wxSizeEvent& e);
		void OnMove(wxMoveEvent& e);

		// �����Լ����䶨��Ϊ�麯��
		virtual void OnPropertyGridChange (wxPropertyGridEvent &event);
		virtual void OnPropertyGridChanging (wxPropertyGridEvent &event);
		virtual void OnPropertyGridSelect (wxPropertyGridEvent &event);

		// PX2 Event
		virtual void DoEnter ();
		virtual void OnEvent (PX2::Event *event);
		virtual void DoLeave ();

	protected:
		PropertyGrid ();
		void Create ();

		DECLARE_EVENT_TABLE()

		std::vector<PropertyPagePtr> mPages;
		PropertyPagePtr mActivePage;

public_internal:
		wxPropertyGridManager *mPropGridManager;
		wxPropertyGrid *mPropGrid;
	};

}

#endif