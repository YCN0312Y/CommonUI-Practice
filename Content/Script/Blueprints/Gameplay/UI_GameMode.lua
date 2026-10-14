--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_UI_GameMode_C
local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

-- function M:UserConstructionScript()
-- end

function M:ReceiveBeginPlay()

    self.PC = UE.UGameplayStatics.GetPlayerController(self, 0)

    if not UE.UKismetSystemLibrary.IsValid(self.PC) then
        return
    end

    local CameraActors = UE.UGameplayStatics.GetAllActorsOfClass(self, UE.ACameraActor)

    for i = 1, CameraActors:Length() do

        local Camera = CameraActors:Get(i)

        if UE.UKismetSystemLibrary.IsValid(Camera) then

            self.Camera = Camera

            self.PC:SetViewTargetWithBlend(Camera, 0.0, UE.EViewTargetBlendFunction.VTBlend_Linear, 0.0, false)
        end
    end
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

return M
