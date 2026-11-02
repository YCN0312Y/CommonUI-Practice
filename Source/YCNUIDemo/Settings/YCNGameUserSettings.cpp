// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

UYCNGameUserSettings::UYCNGameUserSettings()
	:CachedGameDifficulty(FText::FromString(TEXT("简单"))),
	CachedGlobalVolume(100.f),
	CachedMusicVolume(100.f),
	CachedSFXVolume(100.f),
	CachedVoiceVolume(100.f),
    CachedScreenRatio(FText::FromString(TEXT("自动"))),
    CachedScreenResolution(GetMaxResolutionText())
{

}

UYCNGameUserSettings* UYCNGameUserSettings::Get()
{
    return GEngine ? Cast<UYCNGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

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

FText UYCNGameUserSettings::GetResolutionText(const FIntPoint& InResolution)
{
    return FText::FromString(FString::Printf(TEXT("%dx%d"), InResolution.X, InResolution.Y));
}

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
    ApplyResolutionSettings(false);
}

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

FText UYCNGameUserSettings::GetCachedScreenRatio()
{
    if (!IsSupportedScreenRatio(CachedScreenRatio))
    {
        return FText::FromString(TEXT("自动"));
    }

    return CachedScreenRatio;
}

void UYCNGameUserSettings::SetCachedScreenResolution(const FText& InCachedScreenResolution)
{
    FIntPoint TargetResolution = FIntPoint::ZeroValue;

    bool bFound = false;

    for (const FIntPoint& AvailableResolution : GetAvailableResolution())
    {
        if (GetResolutionText(AvailableResolution).EqualTo(InCachedScreenResolution))
        {
            TargetResolution = AvailableResolution;
            bFound = true;
            break;
        }
    }

    if (!bFound)return;
    
    //先记录选择：无边框/全屏下分辨率由显示器决定，但偏好要保留下来
    CachedScreenResolution = InCachedScreenResolution;

    //只有窗口模式才由分辨率决定窗口大小
    if (GetFullscreenMode() != EWindowMode::Windowed) return;

    SetScreenResolution(TargetResolution);
    ApplyResolutionSettings(false);
}

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
