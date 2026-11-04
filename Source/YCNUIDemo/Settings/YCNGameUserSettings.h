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

protected:
	virtual void ApplyNonResolutionSettings() override;

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

	//帧率上限
	UPROPERTY(Config)
	float CachedFrameRateLimit;

	//垂直同步
	UPROPERTY(Config)
	bool bVerticalSync;

	//动态模糊
	UPROPERTY(Config)
	FText CachedDynamicFuzzy;

	//锐化强度
	UPROPERTY(Config)
	float CachedSharpenAmount;

	//字幕
	UPROPERTY(Config)
	bool bSubtitle;

	//超分辨率采样
	UPROPERTY(Config)
	int32 CachedAAMethod;

	//帧生成
	UPROPERTY(Config)
	int32 CachedMSAACount;

private:
	//获取最大分辨率
	bool GetMaxSupportedResolution(FIntPoint& OutResolution) const;

	//是否是合法的宽高比选项
	static bool IsSupportedScreenRatio(const FText& InScreenRatio);

	//把宽高比文本解析成目标分辨率
	bool ResolveScreenRatio(const FText& InScreenRatio, FIntPoint& OutResolution) const;

	//在可用分辨率列表里查找文本对应的分辨率
	bool FindAvailableResolutionByText(const FText& InResolutionText, FIntPoint& OutResolution) const;

	//设置控制台变量
	void SetConsoleVariable(const FString& InVariableString, float InValue);

	void ApplyAntiAliasingSettings();

public:
	//获取最大分辨率字符串
	FText GetMaxResolutionText();

	//获取可用分辨率（按像素数升序）
	TArray<FIntPoint> GetAvailableResolution() const;

	//把分辨率转成显示文本（格式：1920x1080）
	static FText GetResolutionText(const FIntPoint& InResolution);

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

	//显示模式
	void SetCachedDisplayMode(const FText& InCachedDisplayMode);
	FText GetCahcedDisplayMode();

	//屏幕宽高比
	void SetCachedScreenRatio(const FText& InCachedScreenRatio);
	FText GetCachedScreenRatio();

	//屏幕分辨率
	void SetCachedScreenResolution(const FText& InCachedScreenResolution);
	FText GetCachedScreenResolution();

	//帧率上限
	void SetCachedFrameRateLimit(const FText& InCachedFrameRateLimit);
	FText GetCachedFrameRateLimit();

	//垂直同步
	void SetVerticalSync(const FText& InVerticalSync);
	FText GetVerticalSync();

	//动态模糊
	void SetCachedDynamicFuzzy(const FText& InCachedDynamicFuzzy);
	FText GetCachedDynamicFuzzy();

	//锐化强度
	void SetCachedSharpenAmount(float InCachedSharpenAmount);
	float GetCachedSharpenAmount()const;

	//锐化强度
	void SetbSubtitle(const FText& InSubtitle);
	FText GetbSubtitle()const;

	//超采样清晰度
	void SetRenderResolutionPercent(float InValue);
	float GetRenderResolutionPercent()const;

	//超分辨率采样
	void SetCachedAAMethod(const FText& InValue);
	FText GetCachedAAMethod() const;

	//帧生成
	void SetFrameGeneration(const FText& InValue);
	FText GetFrameGeneration() const;
};
