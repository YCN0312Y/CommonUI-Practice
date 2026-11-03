// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/DefaultValueHelper.h"

UYCNGameUserSettings::UYCNGameUserSettings()
    :CachedGameDifficulty(FText::FromString(TEXT("简单"))),
    CachedGlobalVolume(100.f),
    CachedMusicVolume(100.f),
    CachedSFXVolume(100.f),
    CachedVoiceVolume(100.f),
    CachedScreenRatio(FText::FromString(TEXT("自动"))),
    CachedScreenResolution(GetMaxResolutionText()),
    CachedFrameRateLimit(0.f),
    bVerticalSync(false)
{

}

UYCNGameUserSettings* UYCNGameUserSettings::Get()
{
    return GEngine ? Cast<UYCNGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
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
