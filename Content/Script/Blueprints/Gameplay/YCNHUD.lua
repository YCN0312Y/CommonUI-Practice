--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local PushWidget = UE.UYCNAsyncAction_PushWidget
local YCNFunctionLibrary = UE.UYCNFunctionLibrary

---@type BP_YCNHUD_C
local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

-- function M:UserConstructionScript()
-- end

function M:ReceiveBeginPlay()

end

-- function M:ReceiveEndPlay()
-- end

-- function M:ReceiveTick(DeltaSeconds)
-- end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

function M:OnMainStackReady()
    local PC = self:GetOwningPlayerController()
    if PC then
        local WidgetTag = PC.WidgetTagList:Find("HUD")
        local StackTag = PC.StackTagList:Find("HUD")
        if WidgetTag and StackTag then
            if PushWidget and YCNFunctionLibrary then
                local Action = PushWidget.PushSoftWidget(self, StackTag, YCNFunctionLibrary.GetSoftWidgetByTag(WidgetTag))
                if Action then
                    Action:Activate()
                end
            end
        end
    end
end

return M
