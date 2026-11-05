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

	//振动强度
	UPROPERTY(Config)
	float CachedVibrationStrength;

	//字幕
	UPROPERTY(Config)
	bool bSubtitle;

	//超采样清晰度
	UPROPERTY(Config)
	int32 CachedSRSharpness;

	//超分辨率采样
	UPROPERTY(Config)
	int32 CachedAAMethod;

	//帧生成
	UPROPERTY(Config)
	int32 CachedFrameGeneration;

	//全景光线追踪
	UPROPERTY(Config)
	bool bRayTracing;

	//全景光线追踪等级
	UPROPERTY(Config)
	FText CachedRayTracingLevel;

	//待处理画质
	TOptional<Scalability::FQualityLevels> PendingQuality;

private:
	//获取最大分辨率
	bool GetMaxSupportedResolution(FIntPoint& OutResolution) const;

	//是否是合法的宽高比选项
	static bool IsSupportedScreenRatio(const FText& InScreenRatio);

	//把宽高比文本解析成目标分辨率
	bool ResolveScreenRatio(const FText& InScreenRatio, FIntPoint& OutResolution) const;

	//在可用分辨率列表里查找文本对应的分辨率
	bool FindAvailableResolutionByText(const FText& InResolutionText, FIntPoint& OutResolution) const;

	//设置控制台变量 GameSetting
	void SetConsoleVariableByGameSetting(const FString& InVariableString, float InValue);

	//设置控制台变量 GameOverride
	void SetConsoleVariableByGameOverride(const FString& InVariableString, float InValue);

	//设置bool值字符串
	void SetBoolValue(const FText& InValue, bool& InbValue);

	//获取bool值字符串
	FText GetBoolValue(bool InbValue);

	int32 SetIntValue(const FText& InValue);

	FText GetIntValue(int32 InValue);

	//编辑待处理画质
	Scalability::FQualityLevels& EditPendingQuality();

	//获取显示质量
	const Scalability::FQualityLevels& GetDisplayedQuality() const;

public:
	//获取最大分辨率字符串
	FText GetMaxResolutionText();

	//获取可用分辨率（按像素数升序）
	TArray<FIntPoint> GetAvailableResolution() const;

	//把分辨率转成显示文本（格式：1920x1080）
	static FText GetResolutionText(const FIntPoint& InResolution);

	//应用路径追踪设置
	UFUNCTION(BlueprintCallable, Category = "YCN User Settings")
	void ApplyPathTracingSetting();

	//应用待处理画质
	bool ApplyPendingQuality();

	//清空待处理质量列表
	void ResetPendingQuality();

	//否有待处理的值未处理就返回真
	bool HasPendingQualityChanges() const;
	/*
	* 回调----------
	*/

	//全局音量
	MAKE_SET_GET(float, CachedGlobalVolume);

	//音乐音量
	MAKE_SET_GET(float, CachedMusicVolume);

	//音效音量
	MAKE_SET_GET(float, CachedSFXVolume);

	//对话音量
	MAKE_SET_GET(float, CachedVoiceVolume);

	//振动强度
	MAKE_SET_GET(float, CachedVibrationStrength);
	
	//超采样清晰度
	MAKE_SET_GET(int32, CachedSRSharpness);

	//全景光线追踪等级
	MAKE_SET_GET(FText, CachedRayTracingLevel);

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

	void SetbSubtitle(const FText& InValue);
	FText GetbSubtitle();

	//超分辨率采样
	void SetCachedAAMethod(const FText& InValue);
	FText GetCachedAAMethod() const;

	//帧生成
	void SetCachedFrameGeneration(const FText& InValue);
	FText GetCachedFrameGeneration() const;

	//全景光线追踪
	void SetbRayTracing(const FText& InValue);
	FText GetbRayTracing();
	bool IsRayTracingEnabled() const { return bRayTracing; }

	//画质等级
	void SetOverallScalabilityLevelOverride(const FText& InValue);
	FText GetOverallScalabilityLevelOverride();

	//视野距离
	void SetViewDistanceQualityOverride(const FText& InValue);
	FText GetViewDistanceQualityOverride();

	//抗锯齿
	void SetAntiAliasingQualityOverride(const FText& InValue);
	FText GetAntiAliasingQualityOverride();

	//后期处理
	void SetPostProcessingQualityOverride(const FText& InValue);
	FText GetPostProcessingQualityOverride();

	//阴影质量
	void SetShadowQualityOverride(const FText& InValue);
	FText GetShadowQualityOverride();

	//贴图质量
	void SetTextureQualityOverride(const FText& InValue);
	FText GetTextureQualityOverride();

	//特效质量
	void SetVisualEffectQualityOverride(const FText& InValue);
	FText GetVisualEffectQualityOverride();

	//植被质量
	void SetFoliageQualityOverride(const FText& InValue);
	FText GetFoliageQualityOverride();

	//全局光照
	void SetGlobalIlluminationQualityOverride(const FText& InValue);
	FText GetGlobalIlluminationQualityOverride();

	//反射质量
	void SetReflectionQualityOverride(const FText& InValue);
	FText GetReflectionQualityOverride();

	//着色质量
	void SetShadingQualityOverride(const FText& InValue);
	FText GetShadingQualityOverride();


};
