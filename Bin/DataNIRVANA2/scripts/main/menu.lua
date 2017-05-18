-- menu.lua

-- create main menu
function n_CreateMainMenu()
	-- File
	PX2_APP:Menu_Main_AddMainItem("File", (PX2_LM_EDITOR:V("n_File")))
	PX2_APP:Menu_Main_AddItem("File", "NewProject", PX2_LM_EDITOR:V("n_NewProject").."\tCtrl-N", "n_NewProject", "Proj_NewProject")
	PX2_APP:Menu_Main_AddItem("File", "Open", PX2_LM_EDITOR:V("n_Open").."\tCtrl-O", "n_OpenProject", "Proj_Open")
	PX2_APP:Menu_Main_AddItem("File", "Save", PX2_LM_EDITOR:V("n_Save").."\tCtrl-S", "n_SaveProject", "Proj_Save")
	PX2_APP:Menu_Main_AddItem("File", "Close", PX2_LM_EDITOR:V("n_Close"), "n_CloseProject", "Proj_Close")
	PX2_APP:Menu_Main_AddItemSeparater("File")
	PX2_APP:Menu_Main_AddItem("File", "NewScene", PX2_LM_EDITOR:V("n_NewScene").."\tCtrl-N", "n_NewScene", "Proj_Scene_NewScene")
	PX2_APP:Menu_Main_AddItem("File", "Open", PX2_LM_EDITOR:V("n_Open"), "n_OpenScene", "Proj_Scene_Open")
	PX2_APP:Menu_Main_AddItem("File", "Save", PX2_LM_EDITOR:V("n_Save"), "n_SaveScene", "Proj_Scene_Save")
	PX2_APP:Menu_Main_AddItem("File", "SaveAs", PX2_LM_EDITOR:V("n_SaveAs"), "n_SaveSceneAs", "Proj_Scene_SaveAs")
	PX2_APP:Menu_Main_AddItem("File", "Close", PX2_LM_EDITOR:V("n_Close"), "n_CloseScene", "Proj_Scene_Close")
	PX2_APP:Menu_Main_AddItemSeparater("File")
	PX2_APP:Menu_Main_AddItem("File", "Exit", PX2_LM_EDITOR:V("n_Exit"), "n_Exit", "")
	
	-- Edit
	PX2_APP:Menu_Main_AddMainItem("Edit", (PX2_LM_EDITOR:V("n_Edit")))
	PX2_APP:Menu_Main_AddItem("Edit", "UnDo", PX2_LM_EDITOR:V("n_UnDo").."\tCtrl-Z", "n_UnDo", "")
	PX2_APP:Menu_Main_AddItem("Edit", "ReDo", PX2_LM_EDITOR:V("n_ReDo").."\tCtrl-R", "n_ReDo", "")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Copy", PX2_LM_EDITOR:V("n_Copy").."\tCtrl-C", "n_Copy", "")
	PX2_APP:Menu_Main_AddItem("Edit", "Paste", PX2_LM_EDITOR:V("n_Paste").."\tCtrl-V", "n_Paste", "")
	PX2_APP:Menu_Main_AddItem("Edit", "Clone", PX2_LM_EDITOR:V("n_Clone").."\tCtrl-D", "n_Clone", "")
	PX2_APP:Menu_Main_AddItem("Edit", "Delete", PX2_LM_EDITOR:V("n_Delete").."\tDelete", "n_DeleteSelection", "")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Import", PX2_LM_EDITOR:V("n_Import").."\tCtrl-I", "n_Import", "")
	PX2_APP:Menu_Main_AddItem("Edit", "Export", PX2_LM_EDITOR:V("n_Export").."\tCtrl-E", "n_Export", "")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "Select", PX2_LM_EDITOR:V("n_Select"), "", "")
	PX2_APP:Menu_Main_AddItem("Edit", "Translate", PX2_LM_EDITOR:V("n_Translate"), "", "")
	PX2_APP:Menu_Main_AddItem("Edit", "Rotate", PX2_LM_EDITOR:V("n_Rotate"), "", "")
	PX2_APP:Menu_Main_AddItem("Edit", "Scale", PX2_LM_EDITOR:V("n_Scale"), "", "")
	PX2_APP:Menu_Main_AddItemSeparater("Edit")
	PX2_APP:Menu_Main_AddItem("Edit", "PlayAndStop", PX2_LM_EDITOR:V("n_PlayAndStop"), "", "")
	PX2_APP:Menu_Main_AddItem("Edit", "ResetPlay", PX2_LM_EDITOR:V("n_ResetPlay"), "", "")
	

end