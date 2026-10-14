--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_CUW_MainStack_C
local M = UnLua.Class()

function M:OnInitialize()
end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self:RegisterStackTag("HUD", self.Stack_HUD)
    self:RegisterStackTag("Menu", self.Stack_Menu)
    self:RegisterStackTag("Modal", self.Stack_Modal)
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:RegisterStackTag(StackName, StackWidget)
    local Tag = self.StackTag:Find(StackName)

    if Tag then
        self:AddWidgetToStackList(Tag, StackWidget)
    end

end

return M
