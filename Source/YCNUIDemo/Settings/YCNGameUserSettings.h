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
	UYCNGameUserSettings();

	static UYCNGameUserSettings* Get();

private:
	//游戏难度
	UPROPERTY(Config)
	FText CachedGameDifficulty;

	//全局音量
	UPROPERTY(Config)
	float CachedGlobalVolume;

	//音乐音量
	UPROPERTY(Config)
	float CachedMusicVolume;

	//音效音量
	UPROPERTY(Config)
	float CachedSFXVolume;

	//对话音量
	UPROPERTY(Config)
	float CachedVoiceVolume;

	//屏幕高宽比
	UPROPERTY(Config)
	FText CachedScreenRatio;

	//屏幕分辨率
	UPROPERTY(Config)
	FText CachedScreenResolution;

private:
	//获取最大分辨率
	bool GetMaxSupportedResolution(FIntPoint& OutResolution) const;

	//是否是合法的宽高比选项
	static bool IsSupportedScreenRatio(const FText& InScreenRatio);

	//把宽高比文本解析成目标分辨率
	bool ResolveScreenRatio(const FText& InScreenRatio, FIntPoint& OutResolution) const;

	//获取可用分辨率（按像素数升序）
	TArray<FIntPoint> GetAvailableResolution() const;

	//把分辨率转成显示文本（格式：1920x1080）
	static FText GetResolutionText(const FIntPoint& InResolution);

public:
	//获取最大分辨率字符串
	FText GetMaxResolutionText();

	/*
	* 回调----------
	*/

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

	//设置缓存显示模式
	void SetCachedDisplayMode(const FText& InCachedDisplayMode);

	//获取缓存显示模式
	FText GetCahcedDisplayMode();

	//设置缓存屏幕宽高比
	void SetCachedScreenRatio(const FText& InCachedScreenRatio);

	//获取缓存屏幕宽高比
	FText GetCachedScreenRatio();

	//设置缓存屏幕分辨率
	void SetCachedScreenResolution(const FText& InCachedScreenResolution);

	//获取缓存屏幕分辨率
	FText GetCachedScreenResolution();


};
