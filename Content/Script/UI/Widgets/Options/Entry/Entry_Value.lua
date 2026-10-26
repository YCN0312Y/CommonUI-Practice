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

function M:BP_OnHovered(bIsHovered, bIsSelected)

    if bIsHovered then
        if not bIsSelected then
            self:SwitchItemStatus(true)
        end
    else
        if bIsSelected then
            self:SwitchItemStatus(true)
        else
            self:SwitchItemStatus(false)
        end
    end
    
end
return M
