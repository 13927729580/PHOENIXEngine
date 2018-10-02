// PX2EditorEventType.hpp

#ifndef PX2EDITOREVENTTYPE_HPP
#define PX2EDITOREVENTTYPE_HPP

#include "PX2EditorPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_EDITOR_ITEM EditorEventSpace)
	PX2_EVENT(N_NewProject)
	PX2_EVENT(N_OpenProject)
	PX2_EVENT(N_SaveProject)
	PX2_EVENT(N_CloseProject)
	PX2_EVENT(N_NewScene)
	PX2_EVENT(N_OpenScene)
	PX2_EVENT(N_SaveScene)
	PX2_EVENT(N_SaveSceneAs)
	PX2_EVENT(N_CloseScene)
	PX2_EVENT(N_Exit)
	PX2_EVENT(N_PlayInWindow)
	PX2_EVENT(N_ObjectNameChanged)
	PX2_EVENT(N_PlayTip)
	PX2_EVENT(N_Import)
	PX2_EVENT(N_Export)
	PX2_EVENT(N_AddTool)

	PX2_EVENT(Undo)
	PX2_EVENT(Redo)
	PX2_EVENT(Copy)
	PX2_EVENT(Paste)
	PX2_EVENT(Clone)
	PX2_EVENT(Delete)

	PX2_EVENT(ChangeResDir)
	PX2_EVENT(SelectRes)
	PX2_EVENT(RefreshRes)
	PX2_EVENT(ClearRes)
	PX2_EVENT(OpenRes)
	PX2_EVENT(UpRes)
	PX2_EVENT(DownRes)
	PX2_EVENT(SaveText)

	PX2_EVENT(OpenContentRes)
	PX2_EVENT(OpenContentResTemp)
	PX2_EVENT(RemoveContentRes)

	PX2_EVENT(SetPreViewObject)

	PX2_EVENT(SetInspectorView)

	PX2_EVENT(SetProjectTreeLevel)

	PX2_EVENT(SetCopyText)

	PX2_EVENT(ObjectTransformChanged)
	PX2_EVENT(ObjectSizeChanged)

	PX2_EVENT(SetEditType)
	PX2_EVENT(SetEditAxisMode)
	PX2_EVENT(SetEditMode)
	PX2_EVENT(SetRenderMode)
	PX2_EVENT(SetCameraMode)
	PX2_EVENT(SetSimuRenderViewMode)

	PX2_EVENT(SetTheme)
	
	PX2_EVENT(SetPickPos)
	PX2_EVENT(SetPickPosUI)

	PX2_EVENT(FindActorInProjectTree)
	PX2_EVENT(FindObjectInResTree)

	PX2_EVENT(SelectTerrainPage)

	PX2_EVENT(TimeLine_FitHor)
	PX2_EVENT(TimeLine_FitVer)
	PX2_EVENT(TimeLine_FitAll)
	PX2_EVENT(TimeLine_Fit_Selected)
	PX2_EVENT(TimeLine_Pan)
	PX2_EVENT(TimeLine_Zoom)
	PX2_EVENT(TimeLine_CurveMode)

	PX2_EVENT(TimeLine_AddPoint)
	PX2_EVENT(TimeLine_SetInValue)
	PX2_EVENT(TimeLine_SetOutValue)

	PX2_EVENT(CurveChangedByPropertyGrid)

	PX2_DECLARE_EVENT_END(EditorEventSpace)

}

#endif