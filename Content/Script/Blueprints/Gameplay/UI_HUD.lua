--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local PushWidget = UE.UYCNAsyncAction_PushWidget
local YCNFunctionLibrary = UE.UYCNFunctionLibrary

---@type BP_UI_HUD_C
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

function M:OnMainStackReady()
    
    local PC = self:GetOwningPlayerController()
    if not PC then
        print("启动失败：PlayerController 无效")
        return
    end

    -- 启动时先显示加载窗口。
    local WidgetTag = PC.WidgetTagList:Find("StartupLoading")

    -- 加载窗口覆盖在 Modal 层。
    local StackTag = PC.StackTagList:Find("Modal")

    if not WidgetTag or not StackTag then
        print("启动失败：StartupLoading 或 Modal 标签未配置")
        return
    end

    local SoftWidget =
        YCNFunctionLibrary.GetSoftWidgetByTag(WidgetTag)

    local Action =
        PushWidget.PushSoftWidget(self, StackTag, SoftWidget)

    if not Action then
        print("启动失败：无法创建加载窗口推送请求")
        return
    end

    Action:Activate()
end

return M
