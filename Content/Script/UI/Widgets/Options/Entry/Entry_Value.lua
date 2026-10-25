--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_CUW_Entry_Value_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end
local EntryKeyClass = UE.UClass.Load("/Game/UI/Components/Button/WBP_Button_Option_EntryKey.WBP_Button_Option_EntryKey_C")

-- 鼠标进入控件时调用
function M:OnMouseEnter(MyGeometry, MouseEvent)

    local DropdownButton = self.CommonButton_DropdownButton

    if DropdownButton and EntryKeyClass then
        local EntryKeyWidget = DropdownButton:Cast(EntryKeyClass)

        if EntryKeyWidget then
            if EntryKeyWidget.IncoSelectedColor then
                EntryKeyWidget:SetButtonIconColor(EntryKeyWidget.IncoSelectedColor)
            end
        end
    end

    if self.CommonText_SelectedText and self.TextStyle_Selected then
        self.CommonText_SelectedText:SetStyle(self.TextStyle_Selected)
    end

end

function M:OnMouseLeave()

    local DropdownButton = self.CommonButton_DropdownButton

    if DropdownButton and EntryKeyClass then
        local EntryKeyWidget = DropdownButton:Cast(EntryKeyClass)

        if EntryKeyWidget then
            if EntryKeyWidget.IncoDefaultColor then
                EntryKeyWidget:SetButtonIconColor(EntryKeyWidget.IncoDefaultColor)
            end
        end
    end

    if self.CommonText_SelectedText and self.TextStyle_Default then
        self.CommonText_SelectedText:SetStyle(self.TextStyle_Default)
    end

end

return M
