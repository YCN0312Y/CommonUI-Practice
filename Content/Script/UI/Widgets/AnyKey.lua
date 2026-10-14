--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local YCNFunctionLibrary = UE.UYCNFunctionLibrary
local YCNPushWidget = UE.UYCNAsyncAction_PushWidget
local WidgetLibrary = UE.UWidgetBlueprintLibrary

---@type WBP_CAW_AnyKey_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    if self.AnyKeyAnim then
        self:PlayAnimation(self.AnyKeyAnim, 0, 0, UE.EUMGSequencePlayMode.Forward, 0.25, false)
    end
end

function M:BP_OnActivated()

    local PC = self:GetOwningPlayer()
    if PC then
        self:SetUserFocus(PC)
    end
end

function M:BP_GetDesiredFocusTarget()
    return self
end

function M:OnKeyDown(MyGeometry, InKeyEvent)
    self:EnterMainMenu()
    return WidgetLibrary.Handled()
end

function M:OnMouseButtonDown(MyGeometry, MouseEvent)
    self:EnterMainMenu()
    return WidgetLibrary.Handled()
end

function M:EnterMainMenu()
    local PC = self:GetOwningPlayer()

    local StackTag = PC.StackTagList:Find("Menu")
    local WidgetTag = PC.WidgetTagList:Find("MainMenu")
    if StackTag and StackTag and WidgetTag then
        local SoftWidget = YCNFunctionLibrary.GetSoftWidgetByTag(WidgetTag)

        local PushAction = YCNPushWidget.PushSoftWidget(self, StackTag, SoftWidget)

        if PushAction then
            PushAction:Activate()
        end

    end
end

return M
