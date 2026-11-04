// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/DefaultValueHelper.h"
#include "HAL/IConsoleManager.h"

UYCNGameUserSettings::UYCNGameUserSettings()
    :CachedGameDifficulty(FText::FromString(TEXT("简单"))),
    CachedGlobalVolume(100.f),
    CachedMusicVolume(100.f),
    CachedSFXVolume(100.f),
    CachedVoiceVolume(100.f),
    CachedScreenRatio(FText::FromString(TEXT("自动"))),
    CachedScreenResolution(GetMaxResolutionText()),
    CachedFrameRateLimit(0.f),
    bVerticalSync(false),
    CachedDynamicFuzzy(FText::FromString(TEXT("自动"))),
    CachedSharpenAmount(5.f),
    bSubtitle(true),
    CachedAAMethod(4),
    CachedMSAACount(4)
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

void UYCNGameUserSettings::SetConsoleVariable(const FString& InVariableString, float InValue)
{
    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*InVariableString))
    {
        CVar->Set(InValue, ECVF_SetByGameSetting);
    }
}

void UYCNGameUserSettings::ApplyAntiAliasingSettings()
{
    // 验证配置文件加载进来的数值。
    if (CachedAAMethod < 0 || CachedAAMethod > 4)
    {
        CachedAAMethod = 4;
    }

    if (CachedMSAACount != 1 && CachedMSAACount != 2 && CachedMSAACount != 4 && CachedMSAACount != 8)
    {
        CachedMSAACount = 4;
    }

    // 防止旧配置在不支持桌面 MSAA 的项目中继续使用 MSAA。
    if (CachedAAMethod == 3)
    {
        const IConsoleVariable* ForwardShading = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ForwardShading"));

        if (!ForwardShading || ForwardShading->GetInt() == 0)
        {
            CachedAAMethod = 4;
        }
    }

    if (IConsoleVariable* SampleCount = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MSAACount")))
    {
        SampleCount->Set(CachedMSAACount, ECVF_SetByGameSetting);
    }

    if (IConsoleVariable* Method = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod")))
    {
        Method->Set(CachedAAMethod, ECVF_SetByGameSetting);
    }
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

//设置缓存显示模式
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

//获取缓存显示模式
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

//设置缓存屏幕宽高比
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

//获取缓存屏幕宽高比
FText UYCNGameUserSettings::GetCachedScreenRatio()
{
    if (!IsSupportedScreenRatio(CachedScreenRatio))
    {
        return FText::FromString(TEXT("自动"));
    }

    return CachedScreenRatio;
}

//设置缓存屏幕分辨率
void UYCNGameUserSettings::SetCachedScreenResolution(const FText& InCachedScreenResolution)
{
    FIntPoint TargetResolution = FIntPoint::ZeroValue;
    if (!FindAvailableResolutionByText(InCachedScreenResolution, TargetResolution)) return;

    CachedScreenResolution = InCachedScreenResolution;

    if (GetFullscreenMode() != EWindowMode::Windowed) return;

    SetScreenResolution(TargetResolution);
    ApplyResolutionSettings(false);
}

//获取缓存屏幕分辨率
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
    if (InVerticalSync.EqualTo(FText::FromString(TEXT("开启"))))
    {
        bVerticalSync = true;
    }
    else if (InVerticalSync.EqualTo(FText::FromString(TEXT("关闭"))))
    {
        bVerticalSync = false;
    }
}

FText UYCNGameUserSettings::GetVerticalSync()
{
    return bVerticalSync ? FText::FromString(TEXT("开启")) : FText::FromString(TEXT("关闭"));
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

    SetConsoleVariable(TEXT("r.MotionBlur.Amount"), Amount);
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

    SetConsoleVariable(TEXT("r.Tonemapper.Sharpen"), EngineSharpenAmount);
}

float UYCNGameUserSettings::GetCachedSharpenAmount() const
{
    return CachedSharpenAmount;
}

void UYCNGameUserSettings::SetbSubtitle(const FText& InSubtitle)
{
    if (InSubtitle.EqualTo(FText::FromString(TEXT("开启"))))
    {
        bSubtitle = true;
    }
    else if (InSubtitle.EqualTo(FText::FromString(TEXT("关闭"))))
    {
        bSubtitle = false;
    }
}

FText UYCNGameUserSettings::GetbSubtitle() const
{
    return bSubtitle ? FText::FromString(TEXT("开启")) : FText::FromString(TEXT("关闭"));
}

void UYCNGameUserSettings::SetRenderResolutionPercent(float InValue)
{
    if (!FMath::IsFinite(InValue)) return;
    
    const float ClampedValue = FMath::Clamp(InValue, 25.0f, 100.0f);
    const float ResolutionPercent = static_cast<float>(FMath::RoundToInt(ClampedValue));
    SetResolutionScaleValueEx(ResolutionPercent);
    ApplyNonResolutionSettings();
}

float UYCNGameUserSettings::GetRenderResolutionPercent() const
{
    float NormalizedValue = 0.0f;
    float CurrentPercent = 100.0f;
    float MinPercent = 0.0f;
    float MaxPercent = 100.0f;

    GetResolutionScaleInformationEx(NormalizedValue, CurrentPercent, MinPercent, MaxPercent);

    return CurrentPercent;
}

void UYCNGameUserSettings::SetCachedAAMethod(const FText& InValue)
{
    int32 NewMethod;

    if (InValue.EqualTo(FText::FromString(TEXT("FXAA"))))
    {
        NewMethod = 1;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("TAA"))))
    {
        NewMethod = 2;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("MSAA"))))
    {
        NewMethod = 3;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("TSR"))))
    {
        NewMethod = 4;
    }
    else
    {
        return;
    }

    if (NewMethod == 3)
    {
        const IConsoleVariable* ForwardShading = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ForwardShading"));

        if (!ForwardShading || ForwardShading->GetInt() == 0) return;
        
    }

    CachedAAMethod = NewMethod;

    ApplyAntiAliasingSettings();
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

void UYCNGameUserSettings::SetFrameGeneration(const FText& InValue)
{
    if (InValue.EqualTo(FText::FromString(TEXT("1X"))))
    {
        CachedMSAACount = 1;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("2X"))))
    {
        CachedMSAACount = 2;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("4X"))))
    {
        CachedMSAACount = 4;
    }
    else if (InValue.EqualTo(FText::FromString(TEXT("8X"))))
    {
        CachedMSAACount = 8;
    }
    else
    {
        return;
    }

    ApplyAntiAliasingSettings();
}

FText UYCNGameUserSettings::GetFrameGeneration() const
{
    switch (CachedMSAACount)
    {
    case 1: return FText::FromString(TEXT("1X"));
    case 2: return FText::FromString(TEXT("2X"));
    case 4: return FText::FromString(TEXT("4X"));
    case 8: return FText::FromString(TEXT("8X"));
    default: return FText::FromString(TEXT("4X"));
    }
}
