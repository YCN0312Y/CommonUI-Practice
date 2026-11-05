// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/DefaultValueHelper.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/KismetRenderingLibrary.h"

UYCNGameUserSettings::UYCNGameUserSettings()
    :CachedGlobalVolume(100.f),
    CachedMusicVolume(100.f),
    CachedSFXVolume(100.f),
    CachedVoiceVolume(100.f),
    CachedScreenRatio(FText::FromString(TEXT("自动"))),
    CachedScreenResolution(GetMaxResolutionText()),
    CachedFrameRateLimit(0.f),
    bVerticalSync(false),
    CachedDynamicFuzzy(FText::FromString(TEXT("自动"))),
    CachedSharpenAmount(5.f),
    CachedVibrationStrength(10.f),
    bSubtitle(true),
    CachedSRSharpness(4),
    CachedAAMethod(4),
    CachedFrameGeneration(4),
    bRayTracing(false),
    CachedRayTracingLevel(FText::FromString(TEXT("弱")))
{

}

UYCNGameUserSettings* UYCNGameUserSettings::Get()
{
    return GEngine ? Cast<UYCNGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void UYCNGameUserSettings::ApplyNonResolutionSettings()
{
    Super::ApplyNonResolutionSettings();

    SetCachedDynamicFuzzy(GetCachedDynamicFuzzy());
    SetCachedSharpenAmount(GetCachedSharpenAmount());
}

//获取最大分辨率
bool UYCNGameUserSettings::GetMaxSupportedResolution(FIntPoint& OutResolution) const
{
    TArray<FIntPoint> Resolutions;

    if (!UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions))return false;
    
    int64 MaxPixels = 0;

    for (const FIntPoint& Resolution : Resolutions)
    {
        const int64 Pixels = static_cast<int64>(Resolution.X) * Resolution.Y;

        if (Pixels > MaxPixels)
        {
            MaxPixels = Pixels;
            OutResolution = Resolution;
        }
    }

    return MaxPixels > 0;
}

//获取最大分辨率文本
FText UYCNGameUserSettings::GetMaxResolutionText()
{
    FIntPoint MaxResolution;
    if (GetMaxSupportedResolution(MaxResolution))
    {
        return GetResolutionText(MaxResolution);
    }
    return FText();
}

bool UYCNGameUserSettings::IsSupportedScreenRatio(const FText& InScreenRatio)
{
    return InScreenRatio.EqualTo(FText::FromString(TEXT("自动")))
        || InScreenRatio.EqualTo(FText::FromString(TEXT("16:9")))
        || InScreenRatio.EqualTo(FText::FromString(TEXT("21:9")));
}

bool UYCNGameUserSettings::ResolveScreenRatio(const FText& InScreenRatio, FIntPoint& OutResolution) const
{
    if (InScreenRatio.EqualTo(FText::FromString(TEXT("自动"))))
    {
        OutResolution = GetDesktopResolution();
    }
    else if (InScreenRatio.EqualTo(FText::FromString(TEXT("16:9"))))
    {
        OutResolution = FIntPoint(1920, 1080);
    }
    else if (InScreenRatio.EqualTo(FText::FromString(TEXT("21:9"))))
    {
        if (!GetMaxSupportedResolution(OutResolution)) return false;
    }
    else
    {
        return false;
    }

    return OutResolution.X > 0 && OutResolution.Y > 0;
}

//根据文本查询可用分辨率
bool UYCNGameUserSettings::FindAvailableResolutionByText(const FText& InResolutionText, FIntPoint& OutResolution) const
{
    for (const FIntPoint& AvailableResolution : GetAvailableResolution())
    {
        if (GetResolutionText(AvailableResolution).EqualTo(InResolutionText))
        {
            OutResolution = AvailableResolution;
            return true;
        }
    }
    return false;
}

void UYCNGameUserSettings::SetConsoleVariableByGameSetting(const FString& InVariableString, float InValue)
{
    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*InVariableString))
    {
        CVar->Set(InValue, ECVF_SetByGameSetting);
    }
}

void UYCNGameUserSettings::SetConsoleVariableByGameOverride(const FString& InVariableString, float InValue)
{
    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*InVariableString))
    {
        CVar->Set(InValue, ECVF_SetByGameOverride);
    }
}

void UYCNGameUserSettings::SetBoolValue(const FText& InValue, bool& InbValue)
{
    if(InValue.EqualTo(FText::FromString(TEXT("开启"))))
    {
        InbValue = true;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("关闭"))))
    {
        InbValue = false;
    }

}

FText UYCNGameUserSettings::GetBoolValue(bool InbValue)
{
    return InbValue ? FText::FromString(TEXT("开启")) : FText::FromString(TEXT("关闭"));
}

int32 UYCNGameUserSettings::SetIntValue(const FText& InValue)
{
    int32 CurrentValue;
    if (InValue.EqualTo(FText::FromString(TEXT("低"))))
    {
        CurrentValue = 0;
    }
    else if(InValue.EqualTo(FText::FromString(TEXT("中"))))
    {
        CurrentValue = 1;
    }
    else if(InValue.EqualTo(FText::FromString(TEXT("高"))))
    {
        CurrentValue = 2;
    }
    else if(InValue.EqualTo(FText::FromString(TEXT("超高"))))
    {
        CurrentValue = 3;
    }
    else if(InValue.EqualTo(FText::FromString(TEXT("影视级"))))
    {
        CurrentValue = 4;
    }
    else
    {
        return 1;
    }
    return CurrentValue;
}

FText UYCNGameUserSettings::GetIntValue(int32 InValue)
{
    switch (InValue)
    {
    case 0:return FText::FromString(TEXT("低"));
    case 1:return FText::FromString(TEXT("中"));
    case 2:return FText::FromString(TEXT("高"));
    case 3:return FText::FromString(TEXT("超高"));
    case 4:return FText::FromString(TEXT("影视级"));
    }
    return FText::GetEmpty();
}

Scalability::FQualityLevels& UYCNGameUserSettings::EditPendingQuality()
{
    if (!PendingQuality.IsSet())
    {
        PendingQuality.Emplace(ScalabilityQuality);
    }
    return PendingQuality.GetValue();
}

const Scalability::FQualityLevels& UYCNGameUserSettings::GetDisplayedQuality() const
{
    return PendingQuality.IsSet() ? PendingQuality.GetValue() : ScalabilityQuality;
}

//获取可用分辨率
TArray<FIntPoint> UYCNGameUserSettings::GetAvailableResolution() const
{
    //结果列表
    TArray<FIntPoint> ResultList;

    //所有分辨率列表
    TArray<FIntPoint> AllResolution;
    if (!UKismetSystemLibrary::GetSupportedFullscreenResolutions(AllResolution))
    {
        return ResultList;
    }

    //预设分辨率只在显示器支持时才加入
    const TArray<FIntPoint> PresetResolutions =
    {
        FIntPoint(1280, 720),
        FIntPoint(1600, 900),
        FIntPoint(1920, 1080)
    };

    for (const FIntPoint& PresetResolution : PresetResolutions)
    {
        if (AllResolution.Contains(PresetResolution))
        {
            ResultList.AddUnique(PresetResolution);
        }
    }

    //加上玩家可支持的最大分辨率
    FIntPoint MaxResolution = FIntPoint::ZeroValue;
    if (GetMaxSupportedResolution(MaxResolution))
    {
        //最大分辨率刚好是上面某一项时不会重复
        ResultList.AddUnique(MaxResolution);
    }

    //按像素数升序，下拉框从低到高排列
    ResultList.Sort([](const FIntPoint& A, const FIntPoint& B)
        {
            return static_cast<int64>(A.X) * A.Y < static_cast<int64>(B.X) * B.Y;
        });

    return ResultList;
}

//分辨率转文本
FText UYCNGameUserSettings::GetResolutionText(const FIntPoint& InResolution)
{
    return FText::FromString(FString::Printf(TEXT("%dx%d"), InResolution.X, InResolution.Y));
}

//全景光线追踪
void UYCNGameUserSettings::ApplyPathTracingSetting()
{
    UKismetRenderingLibrary::EnablePathTracing(bRayTracing);
}

bool UYCNGameUserSettings::ApplyPendingQuality()
{
    if (!PendingQuality.IsSet()) return false;

    //将未处理的画质设置 赋值
    ScalabilityQuality = PendingQuality.GetValue();
    PendingQuality.Reset();

    ApplyNonResolutionSettings();
    SaveSettings();
    return true;
}

void UYCNGameUserSettings::ResetPendingQuality()
{
    PendingQuality.Reset();
}

bool UYCNGameUserSettings::HasPendingQualityChanges() const
{
    //待处理画质有值 而且值 != 整套画质设置
    return PendingQuality.IsSet() && PendingQuality.GetValue() != ScalabilityQuality;
}

//设置显示模式
void UYCNGameUserSettings::SetCachedDisplayMode(const FText& InCachedDisplayMode)
{
    EWindowMode::Type NewMode;

    if (InCachedDisplayMode.EqualTo(FText::FromString(TEXT("无边框模式"))))
    {
        NewMode = EWindowMode::WindowedFullscreen;
    }
    else if (InCachedDisplayMode.EqualTo(FText::FromString(TEXT("窗口模式"))))
    {
        NewMode = EWindowMode::Windowed;
    }
    else
    {
        return;
    }

    SetFullscreenMode(NewMode);

    //切回窗口模式时，按玩家保存的分辨率恢复窗口大小
    if (NewMode == EWindowMode::Windowed)
    {
        FIntPoint TargetResolution = FIntPoint::ZeroValue;

        //优先用保存的分辨率；保存的值无效时（比如换了显示器）退回宽高比
        if (!FindAvailableResolutionByText(GetCachedScreenResolution(), TargetResolution))
        {
            ResolveScreenRatio(GetCachedScreenRatio(), TargetResolution);
        }

        if (TargetResolution.X > 0 && TargetResolution.Y > 0)
        {
            SetScreenResolution(TargetResolution);
        }
    }

    ApplyResolutionSettings(false);
}

//获取显示模式
FText UYCNGameUserSettings::GetCahcedDisplayMode()
{
    switch (GetFullscreenMode())
    {
    case EWindowMode::WindowedFullscreen:
        return FText::FromString(TEXT("无边框模式"));

    case EWindowMode::Windowed:
        return FText::FromString(TEXT("窗口模式"));

    default:
        return FText::GetEmpty();
    }
}

//设置屏幕宽高比
void UYCNGameUserSettings::SetCachedScreenRatio(const FText& InCachedScreenRatio)
{
    if (!IsSupportedScreenRatio(InCachedScreenRatio)) return;
    
    //先记录选择：全屏/无边框下分辨率由显示器决定，但偏好要保留下来
    CachedScreenRatio = InCachedScreenRatio;

    //只有窗口模式才由宽高比决定分辨率
    if (GetFullscreenMode() != EWindowMode::Windowed) return;
    
    FIntPoint TargetResolution = FIntPoint::ZeroValue;
    if (!ResolveScreenRatio(InCachedScreenRatio, TargetResolution))
    {
        UE_LOG(LogTemp, Warning, TEXT("获取目标分辨率失败：%s"), *InCachedScreenRatio.ToString());
        return;
    }

    SetScreenResolution(TargetResolution);
    ApplyResolutionSettings(false);
}

//获取屏幕宽高比
FText UYCNGameUserSettings::GetCachedScreenRatio()
{
    if (!IsSupportedScreenRatio(CachedScreenRatio))
    {
        return FText::FromString(TEXT("自动"));
    }

    return CachedScreenRatio;
}

//设置屏幕分辨率
void UYCNGameUserSettings::SetCachedScreenResolution(const FText& InCachedScreenResolution)
{
    FIntPoint TargetResolution = FIntPoint::ZeroValue;
    if (!FindAvailableResolutionByText(InCachedScreenResolution, TargetResolution)) return;

    CachedScreenResolution = InCachedScreenResolution;

    if (GetFullscreenMode() != EWindowMode::Windowed) return;

    SetScreenResolution(TargetResolution);
    ApplyResolutionSettings(false);
}

//获取屏幕分辨率
FText UYCNGameUserSettings::GetCachedScreenResolution()
{
    if (!CachedScreenResolution.IsEmpty())
    {
        return CachedScreenResolution;
    }

    //首次运行没有保存过：用当前分辨率兜底
    const FIntPoint CurrentResolution = GetScreenResolution();
    if (CurrentResolution.X > 0 && CurrentResolution.Y > 0)
    {
        return GetResolutionText(CurrentResolution);
    }

    return GetResolutionText(GetDesktopResolution());
}

void UYCNGameUserSettings::SetCachedFrameRateLimit(const FText& InCachedFrameRateLimit)
{
	if (InCachedFrameRateLimit.EqualTo(FText::FromString(TEXT("关闭"))))
	{
		CachedFrameRateLimit = 0;
        SetFrameRateLimit(CachedFrameRateLimit);
        ApplySettings(false);
        return;
	}
	if (!FDefaultValueHelper::ParseFloat(*InCachedFrameRateLimit.ToString(), CachedFrameRateLimit))return;

	SetFrameRateLimit(CachedFrameRateLimit);
	ApplySettings(false);
}

FText UYCNGameUserSettings::GetCachedFrameRateLimit()
{
    return CachedFrameRateLimit <= 0.f ? FText::FromString(TEXT("关闭")) : FText::AsNumber(CachedFrameRateLimit);
}

void UYCNGameUserSettings::SetVerticalSync(const FText& InVerticalSync)
{
    SetBoolValue(InVerticalSync, bVerticalSync);
}

FText UYCNGameUserSettings::GetVerticalSync()
{
    return GetBoolValue(bVerticalSync);
}

void UYCNGameUserSettings::SetCachedDynamicFuzzy(const FText& InCachedDynamicFuzzy)
{
    float Amount = -1.0f;
    if (InCachedDynamicFuzzy.EqualTo(FText::FromString(TEXT("自动"))))
    {
        CachedDynamicFuzzy = FText::FromString(TEXT("自动"));
        Amount = -1.0f;
    }
    else if(InCachedDynamicFuzzy.EqualTo(FText::FromString(TEXT("弱"))))
    {
        CachedDynamicFuzzy = FText::FromString(TEXT("弱"));
        Amount = 0.25f;
    }
    else if(InCachedDynamicFuzzy.EqualTo(FText::FromString(TEXT("强"))))
    {
        CachedDynamicFuzzy = FText::FromString(TEXT("强"));
        Amount = 0.75f;
    }
    else if(InCachedDynamicFuzzy.EqualTo(FText::FromString(TEXT("关闭"))))
    {
        CachedDynamicFuzzy = FText::FromString(TEXT("关闭"));
        Amount = 0.0f;
    }
    else
    {
        return;
    }

    SetConsoleVariableByGameSetting(TEXT("r.MotionBlur.Amount"), Amount);
}

FText UYCNGameUserSettings::GetCachedDynamicFuzzy()
{
    return CachedDynamicFuzzy.IsEmpty() ? FText::FromString(TEXT("自动")) : CachedDynamicFuzzy;
}

void UYCNGameUserSettings::SetCachedSharpenAmount(float InCachedSharpenAmount)
{
    if (!FMath::IsFinite(InCachedSharpenAmount))return;
    
    CachedSharpenAmount = FMath::Clamp(InCachedSharpenAmount, 0.0f, 10.0f);

    const float EngineSharpenAmount = CachedSharpenAmount / 10.0f;

    SetConsoleVariableByGameSetting(TEXT("r.Tonemapper.Sharpen"), EngineSharpenAmount);
}

float UYCNGameUserSettings::GetCachedSharpenAmount() const
{
    return CachedSharpenAmount;
}

void UYCNGameUserSettings::SetbSubtitle(const FText& InValue)
{
    SetBoolValue(InValue, bSubtitle);
}

FText UYCNGameUserSettings::GetbSubtitle()
{
    return GetBoolValue(bSubtitle);
}

void UYCNGameUserSettings::SetCachedAAMethod(const FText& InValue)
{
    if (InValue.EqualTo(FText::FromString(TEXT("FXAA"))))
    {
        CachedAAMethod = 1;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("TAA"))))
    {
        CachedAAMethod = 2;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("MSAA"))))
    {
        CachedAAMethod = 3;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("TSR"))))
    {
        CachedAAMethod = 4;
    }
    else
    {
        return;
    }
}

FText UYCNGameUserSettings::GetCachedAAMethod() const
{
    switch (CachedAAMethod)
    {
    case 0: return FText::GetEmpty();
    case 1: return FText::FromString(TEXT("FXAA"));
    case 2: return FText::FromString(TEXT("TAA"));
    case 3: return FText::FromString(TEXT("MSAA"));
    case 4: return FText::FromString(TEXT("TSR"));
    default: return FText::FromString(TEXT("TSR"));
    }
}

void UYCNGameUserSettings::SetCachedFrameGeneration(const FText& InValue)
{
    if (InValue.EqualTo(FText::FromString(TEXT("1X"))))
    {
        CachedFrameGeneration = 1;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("2X"))))
    {
        CachedFrameGeneration = 2;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("4X"))))
    {
        CachedFrameGeneration = 4;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("8X"))))
    {
        CachedFrameGeneration = 8;
    }
    else
    {
        return;
    }

    SetConsoleVariableByGameOverride(TEXT("r.MSAACount"), CachedFrameGeneration);
}

FText UYCNGameUserSettings::GetCachedFrameGeneration() const
{
    switch (CachedFrameGeneration)
    {
    case 1: return FText::FromString(TEXT("1X"));
    case 2: return FText::FromString(TEXT("2X"));
    case 4: return FText::FromString(TEXT("4X"));
    case 8: return FText::FromString(TEXT("8X"));
    default: return FText::FromString(TEXT("4X"));
    }
}

void UYCNGameUserSettings::SetbRayTracing(const FText& InValue)
{
    SetBoolValue(InValue, bRayTracing);

    if (!bRayTracing)
    {
        SetCachedRayTracingLevel(FText::FromString(TEXT("弱")));
    }

    //1、如果真的要打开全景光线追踪就取消注释，并且在 玩家控制器 的 OnPossess 中调用 ApplyPathTracingSetting
    //2、需要在游戏设置中打开 Support Hardware Ray Tracing
    //ApplyPathTracingSetting();
}

FText UYCNGameUserSettings::GetbRayTracing()
{
    return GetBoolValue(bRayTracing);
}

//设置画质等级
void UYCNGameUserSettings::SetOverallScalabilityLevelOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;

    EditPendingQuality().SetFromSingleQualityLevel(Level);
}

//获取画质等级
FText UYCNGameUserSettings::GetOverallScalabilityLevelOverride()
{
    const Scalability::FQualityLevels& Quality = GetDisplayedQuality();
    const int32 Level = Quality.ViewDistanceQuality;

    const bool bSameLevel =
        Quality.AntiAliasingQuality == Level &&
        Quality.PostProcessQuality == Level &&
        Quality.ShadowQuality == Level &&
        Quality.TextureQuality == Level &&
        Quality.EffectsQuality == Level &&
        Quality.FoliageQuality == Level &&
        Quality.GlobalIlluminationQuality == Level &&
        Quality.ReflectionQuality == Level &&
        Quality.ShadingQuality == Level;

    return bSameLevel
        ? GetIntValue(Level)
        : FText::FromString(TEXT("自定义"));
}

//设置视野距离
void UYCNGameUserSettings::SetViewDistanceQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;

    EditPendingQuality().SetViewDistanceQuality(Level);
}

//获取视野距离
FText UYCNGameUserSettings::GetViewDistanceQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().ViewDistanceQuality);
}

//设置抗锯齿
void UYCNGameUserSettings::SetAntiAliasingQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetAntiAliasingQuality(Level);
}

//获取抗锯齿
FText UYCNGameUserSettings::GetAntiAliasingQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().AntiAliasingQuality);
}

//设置后期处理
void UYCNGameUserSettings::SetPostProcessingQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetPostProcessQuality(Level);
}

//获取后期处理
FText UYCNGameUserSettings::GetPostProcessingQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().PostProcessQuality);
}

//设置阴影质量
void UYCNGameUserSettings::SetShadowQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetShadowQuality(Level);
}

//获取阴影质量
FText UYCNGameUserSettings::GetShadowQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().ShadowQuality);
}

//设置贴图质量
void UYCNGameUserSettings::SetTextureQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetTextureQuality(Level);
}

//获取贴图质量
FText UYCNGameUserSettings::GetTextureQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().TextureQuality);
}

//设置特效质量
void UYCNGameUserSettings::SetVisualEffectQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;

    EditPendingQuality().SetEffectsQuality(Level);
}

//获取特效质量
FText UYCNGameUserSettings::GetVisualEffectQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().EffectsQuality);
}

//设置植被质量
void UYCNGameUserSettings::SetFoliageQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetFoliageQuality(Level);
}

//获取植被质量
FText UYCNGameUserSettings::GetFoliageQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().FoliageQuality);
}

//设置全局光照
void UYCNGameUserSettings::SetGlobalIlluminationQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetGlobalIlluminationQuality(Level);
}

//获取全局光照
FText UYCNGameUserSettings::GetGlobalIlluminationQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().GlobalIlluminationQuality);
}

//设置反射质量
void UYCNGameUserSettings::SetReflectionQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetReflectionQuality(Level);
}

//获取反射质量
FText UYCNGameUserSettings::GetReflectionQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().ReflectionQuality);
}

//设置着色质量
void UYCNGameUserSettings::SetShadingQualityOverride(const FText& InValue)
{
    const int32 Level = SetIntValue(InValue);
    if (Level == INDEX_NONE) return;
    EditPendingQuality().SetShadingQuality(Level);
}

//获取着色质量
FText UYCNGameUserSettings::GetShadingQualityOverride()
{
    return GetIntValue(GetDisplayedQuality().ShadingQuality);
}
