--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local PushWidget = UE.UYCNAsyncAction_PushWidget
local PushConfirmation = UE.UYCNAsyncAction_PushConfirmation
local YCNFunctionLibrary = UE.UYCNFunctionLibrary
local WindowsType = UE.EConfirmationWindowsType
local ButtonType = UE.EConfirmationWindowsButtonType

---@type WBP_CAW_MainMenu_C
local M = UnLua.Class()

function M:OnInitialized()

    if self.Story then
        self.Story.OnButtonBaseClicked:Add(self, M.OnClick_Story)
    end

    if self.Quit then
        self.Quit.OnButtonBaseClicked:Add(self, M.OnClick_Quit)
    end

end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:OnClick_Story(Button)
    if not PushWidget then
        print("PushWidget无效!")
        return 
    end
    local PC = self:GetOwningPC()
    if not PC then
        print("PC无效!")
        return
    end
    local LoaddingTag = PC.WidgetTagList:Find("Loadding")
    if not LoaddingTag then
        print("LoaddingTag无效!")
        return
    end
    local StackTag = PC.StackTagList:Find("Modal")
    if not StackTag then
        print("StackTag无效!")
        return
    end
    if not YCNFunctionLibrary then
        print("YCNFunctionLibrary无效!")
        return
    end
    local FoundWidget = YCNFunctionLibrary.GetSoftWidgetByTag(LoaddingTag)
    if not FoundWidget then
        print("FoundWidget无效!")
        return
    end

    local PushAction = PushWidget.PushSoftWidget(self, StackTag, FoundWidget)
    if PushAction then
        PushAction:Activate()
    end
end

function M:OnClick_Quit(Button)

    --确认弹窗推送动作
    if not PushConfirmation then
        print("PushConfirmation无效")
        return
    end

    --函数库
    if not YCNFunctionLibrary then
        print("YCNFunctionLibrary无效")
        return
    end

    --玩家控制器
    self.PC = self:GetOwningPlayer()
    if not self.PC then
        print("PC无效")
        return
    end

    --控件标签
    local WidgetTag = self.PC.WidgetTagList:Find("Window")
    if not WidgetTag then
        print("WidgetTag无效")
        return
    end

    --窗口类型枚举
    if not WindowsType then
        print("WindowsType无效")
        return
    end
    
    --软控件指针
    local SoftWidget = YCNFunctionLibrary.GetSoftWidgetByTag(WidgetTag)
    if not SoftWidget then
        return
    end

    local Action = PushConfirmation.PushConfrimationWindow(self, WindowsType.YesNO, "退出游戏", "确定退出游戏吗？", SoftWidget)

    if Action then
        Action.OnButtonClicked:Add(self, M.OnConfrimationButtonClicked)
        Action:Activate()
    end

end

function M:OnConfrimationButtonClicked(ClickedButtonType)

    if not ButtonType then
        return
    end

    if ClickedButtonType == ButtonType.Yes then
        UE.UKismetSystemLibrary.QuitGame(self, self.PC, UE.EQuitPreference.Quit, false)
    end
    if ClickedButtonType == ButtonType .No then
        
    end
end

return M
