// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/GameInstance/YCNGameInstance.h"

#include "MoviePlayer.h"
#include "UObject/UObjectGlobals.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void UYCNGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::SetupMapLoadingScreen);

	// 在首张地图开始加载前准备画面。
	SetupMapLoadingScreen(FString());
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

	Attributes.WidgetLoadingScreen = SNew(SBorder).HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(NSLOCTEXT(
					"YCNStartup",
					"LoadingMap",
					"Loading..."))
		];

	GetMoviePlayer()->SetupLoadingScreen(Attributes);
}

void UYCNGameInstance::Shutdown()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);

	Super::Shutdown();
}
