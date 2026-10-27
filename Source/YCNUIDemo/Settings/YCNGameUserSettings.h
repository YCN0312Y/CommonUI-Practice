// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "YCNGameUserSettings.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UYCNGameUserSettings();

	static UYCNGameUserSettings* Get();

private:
	//游戏难度文本
	UPROPERTY(Config)
	FText Cached_GameDifficultyText = FText::FromString(TEXT("简单"));
	//默认游戏难度文本
	UPROPERTY(Config)
	FText Default_GameDifficultyText;

public:
	void SetCachedGameDifficultyText(const FText& InGameDifficultyText);
	const FText& GetCachedGameDifficultyText() { return Cached_GameDifficultyText; }

	//设置为默认值
	void SetDataDefaultValue(const FText& InDefaultValue);
	const FText& GetDataDefaultValue();
};
