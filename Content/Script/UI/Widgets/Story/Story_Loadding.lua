--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local KismetSystemLibrary = UE.UKismetSystemLibrary

---@type WBP_CAW_LoadingWindow_C
local M = UnLua.Class()

function M:BP_OnActivated()
    if not KismetSystemLibrary then
        print("KismetSystemLibrary无效!")
        return
    end
    local LoaddingTime = self.LoaddingTime
    if not self.LoaddingTime then
        print("self.LoaddingTime无效!")
        return
    end
    self.TimerHandle = KismetSystemLibrary.K2_SetTimerDelegate(
    { self, self.StopWidget },
    LoaddingTime,
    false
)
end

function M:StopWidget()
    self:aaaa()
end

return M
