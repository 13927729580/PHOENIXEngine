 -- gamepad.lua

local rc_FramePad = nil
local rc_FrameCtrl0 = nil
local rc_FrameCtrl1 = nil
local rc_FrameCtrl2 = nil

local rc_IsPressedLeft0 = false
local rc_IsPressedLeft1 = false
local rc_IsPressedRight0 = false
local rc_IsPressedRight1 = false

local rc_SpeedAdjustGo = 1.0
local rc_SpeedAdjustTurn = 1.0
local rc_LastLeftType = 0
local rc_LastLeftSpeed = 0.0
local rc_LastRightType = 0
local rc_LastRightSpeed = 0.0

local rc_TopBtnsHeight = -140.0

local rc_UICtr_UIMode = 1

function rc_FramePadPage()
    local frameAll = UIFrame:New()
    frameAll:SetAnchorHor(0.0, 1.0)
    frameAll:SetAnchorVer(0.0, 1.0)
    frameAll:SetAnchorParamHor(0.0, 0.0)
    frameAll:SetAnchorParamVer(0.0, 0.0)

    -- slider
    local fVal1 = 0.5
    local fVal2 = 0.5
    local fVal3 = 0.5

    local strlCfg1 = PX2_PROJ:GetConfig("Slider1")
    if ""~=strlCfg1 then
        fVal1 = 0.0 + strlCfg1
    end

    local strlCfg2 = PX2_PROJ:GetConfig("Slider2")
    if ""~=strlCfg2 then
        fVal2 = 0.0 + strlCfg2
    end

    local strlCfg3 = PX2_PROJ:GetConfig("Slider3")
    if ""~=strlCfg3 then
        fVal3 = 0.0 + strlCfg3
    end

    local posVer = 80.0
    for i=1, 3 do
        local name = "slider"..i
        local slider=UISlider:New(name)
        frameAll:AttachChild(slider)
        slider.LocalTransform:SetTranslateY(-2.0)
        slider:GetButSlider():SetStateColorDefaultWhite()
        slider:GetFPicBoxBack():GetUIPicBox():SetPicBoxType(UIPicBox.PBT_NINE)
        slider:GetFPicBoxBack():GetUIPicBox():SetTexCornerSize(12,12,12,12)        
        slider:GetFPicBoxBack():GetUIPicBox():SetTexture("Data/ZERONE/images/directctrl/slider0.png")
        slider:GetButSlider():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/slider1.png")
        slider:GetButSlider():SetSize(60,60)
        slider:SetSliderLength(60)    
        slider:SetAnchorHor(0.5, 0.5)
        slider:SetAnchorParamHor(0.0, 0.0)
        slider:SetAnchorVer(0.0, 0.0)
        slider:SetAnchorParamVer(posVer, posVer)
        slider:SetHeight(60.0)
        slider:SetWidth(300.0)
        slider:SetPercent(1.0)
        slider:SetScriptHandler("rc_UIPadCtrlCallback")
        slider:RegistToScriptSystem()
        if 1==i then
            slider:SetPercent(fVal1, true)
        elseif 2==i then
            slider:SetPercent(fVal2, true)
        elseif 3==i then
            slider:SetPercent(fVal3, true)
        end
        posVer = posVer + 80.0
    end

    -- uiMode
    local butUIMode = UIButton:New("ButUIMode")
    frameAll:AttachChild(butUIMode)
    butUIMode.LocalTransform:SetTranslateY(-1.0)
    butUIMode:SetAnchorHor(0.0, 0.0)
    butUIMode:SetAnchorParamHor(200.0, 200.0)
    butUIMode:SetAnchorVer(1.0, 1.0)
    butUIMode:SetAnchorParamVer(-50.0, -50.0)
    butUIMode:SetPivot(0.5, 0.5)
    butUIMode:CreateAddText(""..PX2_LM_APP:GetValue("UIMode"))
    butUIMode:SetSize(100, 40)
    butUIMode:SetScriptHandler("rc_UIPadCtrlCallback")

    -- btns
    local ledCheck_Sound = UICheckButton:New("CheckButton_Sound")
    frameAll:AttachChild(ledCheck_Sound)
    ledCheck_Sound.LocalTransform:SetTranslateY(-1.0)
    ledCheck_Sound:SetAnchorHor(0.0, 0.0)
    ledCheck_Sound:SetAnchorParamHor(100.0, 100.0)
    ledCheck_Sound:SetAnchorVer(1.0, 1.0)
    ledCheck_Sound:SetAnchorParamVer(rc_TopBtnsHeight, rc_TopBtnsHeight)
    ledCheck_Sound:SetPivot(0.5, 0.5)
    ledCheck_Sound:SetSize(80, 80)
    local fText = ledCheck_Sound:CreateAddText("Sound")
    fText:GetText():SetFontColor(Float3.BLACK)
    ledCheck_Sound:SetScriptHandler("rc_UIPadCtrlCallback")
    ledCheck_Sound:Check(false, false)

    local ledCheck = UICheckButton:New("CheckButton_LED_L")
    frameAll:AttachChild(ledCheck)
    ledCheck.LocalTransform:SetTranslateY(-1.0)
    ledCheck:SetAnchorHor(0.0, 0.0)
    ledCheck:SetAnchorParamHor(200.0, 200.0)
    ledCheck:SetAnchorVer(1.0, 1.0)
    ledCheck:SetAnchorParamVer(rc_TopBtnsHeight, rc_TopBtnsHeight)
    ledCheck:SetPivot(0.5, 0.5)
    ledCheck:SetSize(80, 80)
    local fText = ledCheck:CreateAddText("LED_L")
    fText:GetText():SetFontColor(Float3.BLACK)
    ledCheck:SetScriptHandler("rc_UIPadCtrlCallback")
    ledCheck:Check(false, false)

    local ledCheck_R = UICheckButton:New("CheckButton_LED_R")
    frameAll:AttachChild(ledCheck_R)
    ledCheck_R.LocalTransform:SetTranslateY(-1.0)
    ledCheck_R:SetAnchorHor(0.0, 0.0)
    ledCheck_R:SetAnchorParamHor(300.0, 300.0)
    ledCheck_R:SetAnchorVer(1.0, 1.0)
    ledCheck_R:SetAnchorParamVer(rc_TopBtnsHeight, rc_TopBtnsHeight)
    ledCheck_R:SetPivot(0.5, 0.5)
    ledCheck_R:SetSize(80, 80)
    local fText = ledCheck_R:CreateAddText("LED_R")
    fText:GetText():SetFontColor(Float3.BLACK)
    ledCheck_R:SetScriptHandler("rc_UIPadCtrlCallback")
    ledCheck_R:Check(false, false)

    rc_CreateFrameMode0(frameAll)
    rc_CreateFrameMode1(frameAll)
    rc_CreateFrameMode2(frameAll)
    rc_SetUIMode(1)

    return frameAll
end

function rc_SetUIMode(mode)
    rc_UICtr_UIMode = mode

    rc_FrameCtrl0:Show(false)
    rc_FrameCtrl1:Show(false)
    rc_FrameCtrl2:Show(false)

    if 0==mode then
        rc_FrameCtrl0:Show(true)
    elseif 1==mode then
        rc_FrameCtrl1:Show(true)
    elseif 2==mode then
        rc_FrameCtrl2:Show(true)
    end
end

function rc_CreateFrameMode0(parentFrame)
    local frame = UIFrame:New()
    rc_FrameCtrl0 = frame
    parentFrame:AttachChild(frame)
    frame.LocalTransform:SetTranslateY(-1.0)
    frame:SetAnchorHor(0.0, 1.0)
    frame:SetAnchorVer(0.0, 1.0)
    
    local round = UIRound:New("RoundCtrl")
    frame:AttachChild(round)
    round.LocalTransform:SetTranslateY(-2.0)
    round:SetAnchorHor(0.0, 0.0)
    round:SetAnchorVer(0.0, 0.0)
    round:SetAnchorParamHor(200.0, 200.0)
    round:SetAnchorParamVer(200.0, 200.0)
    round:SetSize(360, 360)
    round:GetButton():SetStateColorDefaultWhite()
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetBack():GetUIPicBox():SetTexture("Data/ZERONE/images/directctrl/round_out.png")
    round:GetButton():SetSize(200, 200)
    round:RegistToScriptSystem()
    round:SetScriptHandler("rc_UIPadCtrlCallback")
    rc_SimpleCtrl_Round = round

    local btnFireLeft = UIButton:New("BtnFire")
    frame:AttachChild(btnFireLeft)
    btnFireLeft.LocalTransform:SetTranslateY(-2.0)
    btnFireLeft:SetAnchorHor(1.0, 1.0)
    btnFireLeft:SetAnchorVer(0.0, 0.0)
    btnFireLeft:SetAnchorParamHor(-280.0, -280.0)
    btnFireLeft:SetAnchorParamVer(200.0, 200.0)
    btnFireLeft:SetStateColorDefaultWhite()
    btnFireLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFireLeft:SetSize(130, 130)
    btnFireLeft:SetScriptHandler("zo_UIPadCtrlCallback")

    local btnFire = UIButton:New("BtnFire")
    frame:AttachChild(btnFire)
    btnFire.LocalTransform:SetTranslateY(-2.0)
    btnFire:SetAnchorHor(1.0, 1.0)
    btnFire:SetAnchorVer(0.0, 0.0)
    btnFire:SetAnchorParamHor(-120.0, -120.0)
    btnFire:SetAnchorParamVer(200.0, 200.0)
    btnFire:SetStateColorDefaultWhite()
    btnFire:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFire:SetSize(130, 130)
    btnFire:SetScriptHandler("zo_UIPadCtrlCallback")
end

function rc_CreateFrameMode1(parentFrame)
    local frame = UIFrame:New()
    rc_FrameCtrl1 = frame
    parentFrame:AttachChild(frame)
    frame.LocalTransform:SetTranslateY(-1.0)
    frame:SetAnchorHor(0.0, 1.0)
    frame:SetAnchorVer(0.0, 1.0)

    local uiFrameLeft = UIFrame:New("FrameLeft")
    frame:AttachChild(uiFrameLeft)
    uiFrameLeft.LocalTransform:SetTranslateY(-1.0)
    uiFrameLeft:SetAnchorHor(0.0, 0.0)
    uiFrameLeft:SetAnchorVer(0.0, 0.0)
    uiFrameLeft:SetAnchorParamHor(200.0, 200.0)
    uiFrameLeft:SetAnchorParamVer(200.0, 200.0)
    uiFrameLeft:SetSize(360, 360)
    local picBack = uiFrameLeft:CreateAddBackgroundPicBox(true, Float3:MakeColor(255, 255, 255))
    picBack:SetTexture("Data/ZERONE/images/directctrl/ctrlbackleftright.png")
	picBack:UseAlphaBlend(true)
    picBack:SetFakeTransparent(false)
    
    local btnLeft0 = UIButton:New("BtnLeft0")
    uiFrameLeft:AttachChild(btnLeft0)
    btnLeft0.LocalTransform:SetTranslateY(-2.0)
    btnLeft0:SetAnchorHor(0.0, 0.5)
    btnLeft0:SetAnchorVer(0.0, 1.0)
    btnLeft0:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btnleft.png")
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btnleft.png")
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btnleftpress.png")
    btnLeft0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnLeft0:SetScriptHandler("rc_UIPadCtrlCallback")
    local btnLeft1 = UIButton:New("BtnLeft1")
    uiFrameLeft:AttachChild(btnLeft1)
    btnLeft1.LocalTransform:SetTranslateY(-2.0)
    btnLeft1:SetAnchorHor(0.5, 1.0)
    btnLeft1:SetAnchorVer(0.0, 1.0)
    btnLeft1:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btnright.png")
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btnright.png")
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btnrightpress.png")
    btnLeft1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnLeft1:SetScriptHandler("rc_UIPadCtrlCallback")

    local uiFrameRight = UIFrame:New("FrameRight")
    frame:AttachChild(uiFrameRight)
    uiFrameRight.LocalTransform:SetTranslateY(-1.0)
    uiFrameRight:SetAnchorHor(1.0, 1.0)
    uiFrameRight:SetAnchorVer(0.0, 0.0)
    uiFrameRight:SetAnchorParamHor(-200.0, -200.0)
    uiFrameRight:SetAnchorParamVer(200.0, 200.0)
    uiFrameRight:SetSize(360, 360)
    local picBack = uiFrameRight:CreateAddBackgroundPicBox(true, Float3:MakeColor(255, 255, 255))
    picBack:SetTexture("Data/ZERONE/images/directctrl/ctrlbackupdown.png")
	picBack:UseAlphaBlend(true)
    picBack:SetFakeTransparent(false)
    
    local btnRight0 = UIButton:New("BtnRight0")
    uiFrameRight:AttachChild(btnRight0)
    btnRight0.LocalTransform:SetTranslateY(-2.0)
    btnRight0:SetAnchorHor(0.0, 1.0)
    btnRight0:SetAnchorVer(0.5, 1.0)
    btnRight0:SetStateColorDefaultWhite()
    btnRight0:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btnup.png")
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btnup.png")
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btnuppress.png")
    btnRight0:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnRight0:SetScriptHandler("rc_UIPadCtrlCallback")
    local btnRight1 = UIButton:New("BtnRight1")
    uiFrameRight:AttachChild(btnRight1)
    btnRight1.LocalTransform:SetTranslateY(-2.0)
    btnRight1:SetAnchorHor(0.0, 1.0)
    btnRight1:SetAnchorVer(0.0, 0.5)
    btnRight1:SetStateColorDefaultWhite()
    btnRight1:SetButType(UIButtonBase.BT_PICBOXSWAP)
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btndown.png")
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_NORMAL):UseAlphaBlendAdd(true)
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/btndown.png")
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_HOVERED):UseAlphaBlendAdd(true)
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/btndownpress.png")
    btnRight1:GetPicBoxAtState(UIButtonBase.BS_PRESSED):UseAlphaBlendAdd(true)
    btnRight1:SetScriptHandler("rc_UIPadCtrlCallback")

    local btnFireLeft = UIButton:New("BtnFire")
    frame:AttachChild(btnFireLeft)
    btnFireLeft.LocalTransform:SetTranslateY(-2.0)
    btnFireLeft:SetAnchorHor(1.0, 1.0)
    btnFireLeft:SetAnchorVer(1.0, 1.0)
    btnFireLeft:SetAnchorParamHor(-280.0, -280.0)
    btnFireLeft:SetAnchorParamVer(rc_TopBtnsHeight, rc_TopBtnsHeight)
    btnFireLeft:SetStateColorDefaultWhite()
    btnFireLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFireLeft:SetSize(130, 130)
    btnFireLeft:SetScriptHandler("zo_UIPadCtrlCallback")

    local btnFire = UIButton:New("BtnFire")
    frame:AttachChild(btnFire)
    btnFire.LocalTransform:SetTranslateY(-2.0)
    btnFire:SetAnchorHor(1.0, 1.0)
    btnFire:SetAnchorVer(1.0, 1.0)
    btnFire:SetAnchorParamHor(-120.0, -120.0)
    btnFire:SetAnchorParamVer(rc_TopBtnsHeight, rc_TopBtnsHeight)
    btnFire:SetStateColorDefaultWhite()
    btnFire:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFire:SetSize(130, 130)
    btnFire:SetScriptHandler("zo_UIPadCtrlCallback")
end

function rc_CreateFrameMode2(parentFrame)
    local frame = UIFrame:New()
    rc_FrameCtrl2 = frame
    parentFrame:AttachChild(frame)
    frame.LocalTransform:SetTranslateY(-1.0)
    frame:SetAnchorHor(0.0, 1.0)
    frame:SetAnchorVer(0.0, 1.0)

    local round = UIRound:New("RoundCtrl")
    frame:AttachChild(round)
    round.LocalTransform:SetTranslateY(-2.0)
    round:SetAnchorHor(0.0, 0.0)
    round:SetAnchorVer(0.0, 0.0)
    round:SetAnchorParamHor(200.0, 200.0)
    round:SetAnchorParamVer(200.0, 200.0)
    round:SetSize(360, 360)
    round:GetButton():SetStateColorDefaultWhite()
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetButton():GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    round:GetBack():GetUIPicBox():SetTexture("Data/ZERONE/images/directctrl/round_out.png")
    round:GetButton():SetSize(200, 200)
    round:RegistToScriptSystem()
    round:SetScriptHandler("rc_UIPadCtrlCallback")

    local roundRight = UIRound:New("RoundCtrl")
    frame:AttachChild(roundRight)
    roundRight.LocalTransform:SetTranslateY(-2.0)
    roundRight:SetAnchorHor(1.0, 1.0)
    roundRight:SetAnchorVer(0.0, 0.0)
    roundRight:SetAnchorParamHor(-200.0, -200.0)
    roundRight:SetAnchorParamVer(200.0, 200.0)
    roundRight:SetSize(360, 360)
    roundRight:GetButton():SetStateColorDefaultWhite()
    roundRight:GetButton():GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    roundRight:GetButton():GetPicBoxAtState(UIButtonBase.BS_HOVERED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    roundRight:GetButton():GetPicBoxAtState(UIButtonBase.BS_PRESSED):SetTexture("Data/ZERONE/images/directctrl/round_in.png")
    roundRight:GetBack():GetUIPicBox():SetTexture("Data/ZERONE/images/directctrl/round_out.png")
    roundRight:GetButton():SetSize(200, 200)
    roundRight:RegistToScriptSystem()
    roundRight:SetScriptHandler("rc_UIPadCtrlCallback")

    local btnFireLeft = UIButton:New("BtnFire")
    frame:AttachChild(btnFireLeft)
    btnFireLeft.LocalTransform:SetTranslateY(-2.0)
    btnFireLeft:SetAnchorHor(1.0, 1.0)
    btnFireLeft:SetAnchorVer(1.0, 1.0)
    btnFireLeft:SetAnchorParamHor(-280.0, -280.0)
    btnFireLeft:SetAnchorParamVer(rc_TopBtnsHeight, rc_TopBtnsHeight)
    btnFireLeft:SetStateColorDefaultWhite()
    btnFireLeft:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFireLeft:SetSize(130, 130)
    btnFireLeft:SetScriptHandler("zo_UIPadCtrlCallback")

    local btnFire = UIButton:New("BtnFire")
    frame:AttachChild(btnFire)
    btnFire.LocalTransform:SetTranslateY(-2.0)
    btnFire:SetAnchorHor(1.0, 1.0)
    btnFire:SetAnchorVer(1.0, 1.0)
    btnFire:SetAnchorParamHor(-120.0, -120.0)
    btnFire:SetAnchorParamVer(rc_TopBtnsHeight, rc_TopBtnsHeight)
    btnFire:SetStateColorDefaultWhite()
    btnFire:GetPicBoxAtState(UIButtonBase.BS_NORMAL):SetTexture("Data/ZERONE/images/directctrl/btn.png")
    btnFire:SetSize(130, 130)
    btnFire:SetScriptHandler("zo_UIPadCtrlCallback")
end

function rc_UIPadCtrlCallback(ptr,callType)
    local obj=Cast:ToO(ptr)
    local name=obj:GetName()
    if UICT_PRESSED==callType then
        PX2_GR:PlayScale(obj)
        if "BtnLeft0" == name then
            rc_IsPressedLeft0 = true
            zo_UpdateButtonsPress()
        end
        if "BtnLeft1" == name then
            rc_IsPressedLeft1 = true
            zo_UpdateButtonsPress()
        end
        if "BtnRight0" == name then
            rc_IsPressedRight0 = true
            zo_UpdateButtonsPress()
        end
        if "BtnRight1" == name then
            rc_IsPressedRight1 = true
            zo_UpdateButtonsPress()
        end
    elseif UICT_RELEASED ==callType then
        PX2_GR:PlayNormal(obj)        
        if "BtnClose" == name then
            rc_FramePad:Show(false)
        elseif "ButUIMode" == name then
            rc_UICtr_UIMode = rc_UICtr_UIMode + 1
            if rc_UICtr_UIMode > 2 then
                rc_UICtr_UIMode = 0
            end

            rc_SetUIMode(rc_UICtr_UIMode)
        end
        
        if "BtnLeft0" == name then
            rc_IsPressedLeft0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnLeft1" == name then
            rc_IsPressedLeft1 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight0" == name then
            rc_IsPressedRight0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight1" == name then
            rc_IsPressedRight1 = false
            zo_UpdateButtonsPress()
        end
    elseif UICT_RELEASED_NOTPICK == callType then
        PX2_GR:PlayNormal(obj)

        if "BtnLeft0" == name then
            rc_IsPressedLeft0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnLeft1" == name then
            rc_IsPressedLeft1 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight0" == name then
            rc_IsPressedRight0 = false
            zo_UpdateButtonsPress()
        end
        if "BtnRight1" == name then
            rc_IsPressedRight1 = false
            zo_UpdateButtonsPress()
        end
    elseif UICT_ROUND_DRAG == callType then
        if "RoundCtrl" == name then      
            local leftMoveType = obj:GetDragingLeftMoveType()
            local leftMoveSpeed = obj:GetDragingLeftMoveSpeed()
            local rightMoveType = obj:GetDragingRightMoveType()
            local rightMoveSpeed = obj:GetDragingRightMoveSpeed()
            if leftMoveType == rightMoveType then
                leftMoveSpeed = leftMoveSpeed * rc_SpeedAdjustGo
                rightMoveSpeed = rightMoveSpeed * rc_SpeedAdjustGo
            else
                leftMoveSpeed = leftMoveSpeed * rc_SpeedAdjustTurn
                rightMoveSpeed = rightMoveSpeed * rc_SpeedAdjustTurn
            end

            if rc_LastLeftType~=leftMoveType or rc_LastLeftSpeed ~=leftMoveSpeed then
                PX2_ARDUINO:Run(0, leftMoveType, leftMoveSpeed)

                rc_LastLeftType = leftMoveType
                rc_LastLeftSpeed = leftMoveSpeed
            end
            if rc_LastRightType~=rightMoveType or rc_LastRightSpeed ~=rightMoveSpeed then
                PX2_ARDUINO:Run(1, rightMoveType, rightMoveSpeed)

                rc_LastRightType = rightMoveType
                rc_LastRightSpeed = rightMoveSpeed
            end
        end
    elseif UICT_ROUND_RELEASED == callType then
        if "RoundCtrl" == name then      
            PX2_ARDUINO:Run(0, 0, 0)
            PX2_ARDUINO:Run(1, 0, 0)

            rc_LastLeftType = 0
            rc_LastLeftSpeed = 0.0
            rc_LastRightType = 0
            rc_LastRightSpeed = 0.0
        end
    elseif UICT_ROUND_RELEASEDNOTPICK == callType then
        if "RoundCtrl" == name then      
            PX2_ARDUINO:Run(0, 0, 0)
            PX2_ARDUINO:Run(1, 0, 0)

            rc_LastLeftType = 0
            rc_LastLeftSpeed = 0.0
            rc_LastRightType = 0
            rc_LastRightSpeed = 0.0
        end
    elseif UICT_SLIDERCHANGED == callType then
        local percent = obj:GetPercent()
        local pos = 180.0*percent
        if not rc_IsFireRobot then
            if "slider3" == name then
                rc_SpeedAdjustTurn = percent
                PX2_PROJ:SetConfig("Slider3", ""..percent)
            elseif "slider2"==name then
                rc_SpeedAdjustGo = percent
                PX2_PROJ:SetConfig("Slider2", ""..percent)
            elseif "slider1"==name then
                PX2_ARDUINO:ServerWrite(0, pos)
                PX2_PROJ:SetConfig("Slider1", ""..percent)
            end
        else
            local pos = 180.0*percent
            if "slider3" == name then
                rc_SpeedAdjustTurn = percent
                rc_SpeedAdjustGo = percent
                PX2_PROJ:SetConfig("Slider3", ""..percent)
            elseif "slider2"==name then
                PX2_ARDUINO:ServerWrite(0, pos)
                PX2_PROJ:SetConfig("Slider2", ""..percent)
            elseif "slider1"==name then
                PX2_ARDUINO:ServerWrite(1, pos)
                PX2_PROJ:SetConfig("Slider1", ""..percent)
            end
        end
    elseif UICT_CHECKED == callType then
        if "CheckButton_LED_L"==name then
            PX2_ARDUINO:DigitalWrite(Arduino.P_11, true)
        elseif "CheckButton_LED_R"==name then
            PX2_ARDUINO:DigitalWrite(Arduino.P_12, true)
        elseif "CheckButton_Sound"==name then
            PX2_ARDUINO:DigitalWrite(Arduino.P_10, true)
        end
    elseif UICT_DISCHECKED == callType then
        if "CheckButton_LED_L"==name then
            PX2_ARDUINO:DigitalWrite(Arduino.P_11, false)
        elseif "CheckButton_LED_R"==name then
            PX2_ARDUINO:DigitalWrite(Arduino.P_12, false)
        elseif "CheckButton_Sound"==name then
            PX2_ARDUINO:DigitalWrite(Arduino.P_10, false)
        end
    end
end

function zo_UpdateButtonsPress()
    if rc_IsPressedLeft0 then
        PX2_ARDUINO:Run(0, 2, 255.0 * rc_SpeedAdjustTurn)
        PX2_ARDUINO:Run(1, 1, 255.0 * rc_SpeedAdjustTurn)
    elseif rc_IsPressedLeft1 then
        PX2_ARDUINO:Run(0, 1, 255.0 * rc_SpeedAdjustTurn)
        PX2_ARDUINO:Run(1, 2, 255.0 * rc_SpeedAdjustTurn)
    else
        if rc_IsPressedRight0 then
            PX2_ARDUINO:Run(0, 1, 255.0 * rc_SpeedAdjustGo)
            PX2_ARDUINO:Run(1, 1, 255.0 * rc_SpeedAdjustGo)
        elseif rc_IsPressedRight1 then
            PX2_ARDUINO:Run(0, 2, 255.0 * rc_SpeedAdjustGo)
            PX2_ARDUINO:Run(1, 2, 255.0 * rc_SpeedAdjustGo)
        else
            PX2_ARDUINO:Run(0, 0, 255.0 * rc_SpeedAdjustGo)
            PX2_ARDUINO:Run(1, 0, 255.0 * rc_SpeedAdjustGo)
        end
    end
end