--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
local YCNFunctionLibrary = UE.UYCNFunctionLibrary
local YCNPushWidget = UE.UYCNAsyncAction_PushWidget

---@type BP_UI_PlayerController_C
local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

-- function M:UserConstructionScript()
-- end

-- function M:ReceiveBeginPlay()
-- end

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

function M:ReceivePossess(PossessedPawn)

    --创建控件
    if not self:IsLocalController() then
        return
    end

    if self.MainStack then
        local MainStackWidget = UE.UWidgetBlueprintLibrary.Create(self, self.MainStack, self)

        local WidgetManager = UE.USubsystemBlueprintLibrary.GetGameInstanceSubsystem(self, UE.UYCNWidgetManagerSubsystem)

        if MainStackWidget and WidgetManager and YCNFunctionLibrary then
            self.bShowMouseCursor = true
            MainStackWidget:AddToViewport(0)

            local StackTag = self.StackTagList:Find("Menu")
            local WidgetTag = self.WidgetTagList:Find("AnyKey")
            WidgetManager:RegisterMainStack(MainStackWidget)

            if YCNPushWidget and StackTag and WidgetTag then
                
                local SoftWidget = YCNFunctionLibrary.GetSoftWidgetByTag(WidgetTag)
                local PushAction = YCNPushWidget.PushSoftWidget(self, StackTag, SoftWidget)
                if PushAction then
                    PushAction:Activate()
                end
            end
        end
    end

end

return M
