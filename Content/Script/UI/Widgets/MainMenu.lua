--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local PushConfirmation = UE.UYCNAsyncAction_PushConfirmation
local YCNFunctionLibrary = UE.UYCNFunctionLibrary
local WindowsType = UE.EConfirmationWindowsType
local ButtonType = UE.EConfirmationWindowsButtonType

---@type WBP_CAW_MainMenu_C
local M = UnLua.Class()

function M:OnInitialized()

    if self.Quit then
        self.Quit.OnButtonBaseClicked:Add(self, M.ClickQuit)
    end

end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:ClickQuit(Button)

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
        Action.OnButtonClicked:Add(self, M.OnButtonClicked)
        Action:Activate()
    end

end

function M:OnButtonClicked(ClickedButtonType)

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
