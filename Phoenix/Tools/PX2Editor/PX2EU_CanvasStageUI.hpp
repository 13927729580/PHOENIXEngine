// PX2EU_CanvasStageUI.hpp

#ifndef PX2EU_CANVASSTAGEUI_HPP
#define PX2EU_CANVASSTATEUI_HPP

#include "PX2EditorPre.hpp"
#include "PX2Canvas.hpp"
#include "PX2Polysegment.hpp"
#include "PX2UIObjectCtrl.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_CanvasStageUI : public Canvas, public Singleton<EU_CanvasStageUI>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CanvasStageUI);

	public:
		EU_CanvasStageUI();
		virtual ~EU_CanvasStageUI();

		virtual void Enable(bool enable);

		virtual void Draw(Renderer *renderer);

	public:
		virtual void OnEvent(Event *event);
		virtual void OnSizeChanged();

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _CreateGridGeometry();
		void _CreateCanvas();
		void _UpdateUIRange();
		void _AdjustCameraPercent();
		void _SetOverCameraNode(CameraNode *cameraNode);
		void _CreateNodeCtrl();

		CanvasPtr mGridCanvas;
		PX2::PolysegmentPtr mProjRangeSegment;
		PX2::NodePtr mRangeNode;

		CanvasPtr mCanvas;

		CanvasPtr mCtrlCanvas;
		UIObjectCtrlPtr mUIObjectCtrl;
		NodePtr mUIObjectNode;

		bool mIsInitSizeChanged;

	public:
		virtual void OnLeftDown(const PickInputData &data);
		virtual void OnLeftUp(const PickInputData &data);
		virtual void OnLeftDClick(const PickInputData &data);
		virtual void OnMiddleDown(const PickInputData &data);
		virtual void OnMiddleUp(const PickInputData &data);
		virtual void OnMouseWheel(const PickInputData &data);
		virtual void OnRightDown(const PickInputData &data);
		virtual void OnRightUp(const PickInputData &data);
		virtual void OnMotion(const PickInputData &data);

	protected:
		virtual std::pair<float, float> CalPixelToWorld();
		void _PanCamera(const float &horz, const float &vert);
		void _ZoomCamera(float zoom);
		void _ClickSelectPos(const APoint &scrPos);  //< ����Pick��
		void _ClickSelectUI(const APoint &scrPos); //< ѡ��UI���

		APoint mSelectPoint;
		APoint mMiddlePickPos;

		// edit menu
	public:
		void CreateMenuOnSelect(const APoint &pos);
	};

	PX2_REGISTER_STREAM(EU_CanvasStageUI);
	typedef Pointer0<EU_CanvasStageUI> EU_CanvasStageUIPtr;

}

#endif