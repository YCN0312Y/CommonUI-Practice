--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Button_Option_MainTab_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:BP_OnHovered()
    local ButtonDisplayIcon = self.LazyImage_ButtonDisplayIcon
    local Color = self.IncoSelectedColor

    if ButtonDisplayIcon and Color then
        ButtonDisplayIcon:SetBrushTintColor(Color)
    end
end

function M:BP_OnUnhovered()
    if self:GetSelected() then
       return 
    end
    local ButtonDisplayIcon = self.LazyImage_ButtonDisplayIcon
    local Color = self.IncoDefaultColor

    if ButtonDisplayIcon and Color then
        ButtonDisplayIcon:SetBrushTintColor(Color)
    end
end

function M:BP_OnSelected()
    local ButtonDisplayIcon = self.LazyImage_ButtonDisplayIcon
    local Color = self.IncoSelectedColor

    if ButtonDisplayIcon and Color then
        ButtonDisplayIcon:SetBrushTintColor(Color)
    end
end

function M:BP_OnDeSelected()
    local ButtonDisplayIcon = self.LazyImage_ButtonDisplayIcon
    local Color = self.IncoDefaultColor

    if ButtonDisplayIcon and Color then
        ButtonDisplayIcon:SetBrushTintColor(Color)
    end
end

return M
