// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "YCNUIDemo/Msic/YCNFunctionHelper.h"
#include "YCNGameUserSettings.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UYCNGameUserSettings* Get();

private:
	//游戏难度
	UPROPERTY(Config)
	FText CachedGameDifficulty;

	//全局音量
	UPROPERTY(Config)
	float CachedGlobalVolume = 100.f;

	//音乐音量
	UPROPERTY(Config)
	float CachedMusicVolume = 100.f;

	//音效音量
	UPROPERTY(Config)
	float CachedSFXVolume = 100.f;

	//对话音量
	UPROPERTY(Config)
	float CachedVoiceVolume = 100.f;

public:
	//游戏难度
	MAKE_SET_GET(FText, CachedGameDifficulty);

	//全局音量
	MAKE_SET_GET(float, CachedGlobalVolume);

	//音乐音量
	MAKE_SET_GET(float, CachedMusicVolume);

	//音效音量
	MAKE_SET_GET(float, CachedSFXVolume);

	//对话音量
	MAKE_SET_GET(float, CachedVoiceVolume);
};
