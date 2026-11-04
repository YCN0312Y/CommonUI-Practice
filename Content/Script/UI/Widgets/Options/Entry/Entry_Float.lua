--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local WidgetLibrary = UE.UWidgetBlueprintLibrary
local GameplayStatics = UE.UGameplayStatics

---@type WBP_CUW_Entry_None_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:OnMouseButtonDown(MyGeometry, MouseEvent)
    if not self.ClickSound or not GameplayStatics then
        return WidgetLibrary:Handled()
    end
    GameplayStatics.PlaySound2D(self, self.ClickSound, 1.0, 1.0, 0.0, nil, nil, true)
    print("WBP_CUW_Entry_None_C")
    return WidgetLibrary:Handled()
end
return M
