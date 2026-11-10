// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/GameInstance/YCNGameInstance.h"

#include "MoviePlayer.h"
#include "UObject/UObjectGlobals.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void UYCNGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::SetupMapLoadingScreen);
}

void UYCNGameInstance::SetupMapLoadingScreen(const FString& MapName)
{
    if (IsRunningDedicatedServer() || !IsMoviePlayerEnabled())
    {
        return;
    }

    FLoadingScreenAttributes Attributes;

    Attributes.bAutoCompleteWhenLoadingCompletes = true;
    Attributes.bWaitForManualStop = false;
    Attributes.bMoviesAreSkippable = false;
    Attributes.MinimumLoadingScreenDisplayTime = 1.0f;

    Attributes.WidgetLoadingScreen =
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
        .BorderBackgroundColor(FLinearColor::Black)
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        [
            SNew(STextBlock)
                .Text(NSLOCTEXT(
                    "YCNLoading",
                    "LoadingMap",
                    "正在加载关卡……"
                ))
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", 28))
                .ColorAndOpacity(FLinearColor::White)
        ];

    GetMoviePlayer()->SetupLoadingScreen(Attributes);
}

void UYCNGameInstance::Shutdown()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);

	Super::Shutdown();
}
