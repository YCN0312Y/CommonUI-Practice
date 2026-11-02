// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_String.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Float.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "GenericPlatform/GenericWindow.h"

void UYCNListData_RegisterData::InitDataList()
{
	AvailableDataList.Reset();

	if (!UserSettings.IsValid())
	{
		UserSettings = UYCNGameUserSettings::Get();
	}

	InitGameSettingsList();
	InitAudioList();
	InitVideoList();
	InitControlList();
	InitAssistList();
}

UYCNListData_Tab* UYCNListData_RegisterData::GetDataByDataID(const FName& InDataID)
{
	for (const TObjectPtr<UYCNListData_Tab>& Data : AvailableDataList)
	{
		if (Data && Data->GetDataID() == InDataID)
		{
			return Data.Get();
		}
	}
	return nullptr;
}

void UYCNListData_RegisterData::InitGameSettingsList()
{
	UYCNListData_Tab* GameplayTab = NewObject<UYCNListData_Tab>();
	if(GameplayTab)
	{
		GameplayTab->SetDataID(FName("GameplayTab"));
		GameplayTab->SetDataDisplayText(SetDataText(TEXT("游戏设置")));

		AvailableDataList.Add(GameplayTab);

		//游戏难度
		{
			UYCNListData_String* GameDifficulty = NewObject<UYCNListData_String>();
			if (GameDifficulty)
			{
				GameDifficulty->SetDataID(FName("GameDifficulty"));
				GameDifficulty->SetDataDisplayText(SetDataText(TEXT("游戏难度")));
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("简单")));
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("正常")));
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("困难")));
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("极难")));
				GameDifficulty->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("简单")));
				const FString GameDifficultyDescription = TEXT("调整游戏的难度。\n\n");
				const FString Easy = TEXT("简单：专注于剧情与探索，敌人较弱且资源充沛，容错率极高，适合新手或轻松体验。\n\n");
				const FString Normal = TEXT("正常：制作组推荐的基准难度，攻防均衡，需要适度运用操作与策略，体验平滑充实。\n\n");
				const FString Difficulty = TEXT("困难：敌人更具侵略性，失误代价更高，考验核心机制掌握与战术应变。\n\n");
				const FString Hardest = TEXT("极难：极限试炼，容错率近乎为零，资源匮乏且敌人致命，专为追求极致挑战的硬核玩家准备。\n\n");
				//绑定值
				GameDifficulty->SetDataDescription(SetDataText(GameDifficultyDescription + Easy + Normal + Difficulty + Hardest));
				BindCachedValue(GameDifficulty, &UYCNGameUserSettings::GetCachedGameDifficulty, &UYCNGameUserSettings::SetCachedGameDifficulty);
				GameplayTab->AddChildData(GameDifficulty);
			}

			UYCNListData_String* Test = NewObject<UYCNListData_String>();
			if (Test)
			{
				Test->SetDataID(FName("Test"));
				Test->SetDataDisplayText(SetDataText(TEXT("测试标签")));
				GameplayTab->AddChildData(Test);
			}
		}
	}
}

void UYCNListData_RegisterData::InitAudioList()
{
	UYCNListData_Tab* AudioTab = NewObject<UYCNListData_Tab>();
	if (AudioTab)
	{
		AudioTab->SetDataID(FName("AudioTab"));
		AudioTab->SetDataDisplayText(SetDataText(TEXT("音频")));

		AvailableDataList.Add(AudioTab);

		//音量
		{
			UYCNListData_Tab* Volume = NewObject<UYCNListData_Tab>();
			if (Volume)
			{
				Volume->SetDataID(FName("Volume"));
				Volume->SetDataDisplayText(SetDataText(TEXT("音量")));

				AudioTab->AddChildData(Volume);

				//全局音量
				UYCNListData_Float* GlobalVolume = NewObject<UYCNListData_Float>();
				if (GlobalVolume)
				{
					GlobalVolume->SetDataID(FName("GlobalVolume"));
					GlobalVolume->SetDataDisplayText(SetDataText(TEXT("全局音量")));
					GlobalVolume->SetDataDescription(SetDataText(TEXT("调整所有声音的总开关。")));
					GlobalVolume->GetSettingBinding().SetOwningDefaultValue(100.f);
					//绑定值
					BindCachedValue(GlobalVolume, &UYCNGameUserSettings::GetCachedGlobalVolume, &UYCNGameUserSettings::SetCachedGlobalVolume);
					Volume->AddChildData(GlobalVolume);
				}

				//音乐音量
				UYCNListData_Float* MusicVolume = NewObject<UYCNListData_Float>();
				if (MusicVolume)
				{
					MusicVolume->SetDataID(FName("MusicVolume"));
					MusicVolume->SetDataDisplayText(SetDataText(TEXT("音乐音量")));
					MusicVolume->SetDataDescription(SetDataText(TEXT("调整音乐音量。")));
					MusicVolume->GetSettingBinding().SetOwningDefaultValue(100.f);
					//绑定值
					BindCachedValue(MusicVolume, &UYCNGameUserSettings::GetCachedMusicVolume, &UYCNGameUserSettings::SetCachedMusicVolume);
					Volume->AddChildData(MusicVolume);
				}

				//音效音量
				UYCNListData_Float* SFXVolume = NewObject<UYCNListData_Float>();
				if (SFXVolume)
				{
					SFXVolume->SetDataID(FName("SFXVolume"));
					SFXVolume->SetDataDisplayText(SetDataText(TEXT("音效音量")));
					SFXVolume->SetDataDescription(SetDataText(TEXT("调整动作、UI点击等特效的音量。")));
					SFXVolume->GetSettingBinding().SetOwningDefaultValue(100.f);
					//绑定值
					BindCachedValue(SFXVolume, &UYCNGameUserSettings::GetCachedSFXVolume, &UYCNGameUserSettings::SetCachedSFXVolume);
					Volume->AddChildData(SFXVolume);
				}

				//对话音量
				UYCNListData_Float* VoiceVolume = NewObject<UYCNListData_Float>();
				if (VoiceVolume)
				{
					VoiceVolume->SetDataID(FName("VoiceVolume"));
					VoiceVolume->SetDataDisplayText(SetDataText(TEXT("对话音量")));
					VoiceVolume->SetDataDescription(SetDataText(TEXT("调整角色配音与旁白的音量。")));
					VoiceVolume->GetSettingBinding().SetOwningDefaultValue(100.f);
					//绑定值
					BindCachedValue(VoiceVolume, &UYCNGameUserSettings::GetCachedVoiceVolume, &UYCNGameUserSettings::SetCachedVoiceVolume);
					Volume->AddChildData(VoiceVolume);
				}
			}
		}
	}
}

void UYCNListData_RegisterData::InitVideoList()
{
	UYCNListData_Tab* VideoTab = NewObject<UYCNListData_Tab>();
	if (VideoTab)
	{
		VideoTab->SetDataID(FName("VideoTab"));
		VideoTab->SetDataDisplayText(SetDataText(TEXT("视频")));

		AvailableDataList.Add(VideoTab);

		UYCNListData_Tab* ScreenTab = NewObject<UYCNListData_Tab>();
		if (ScreenTab)
		{
			ScreenTab->SetDataID(FName("ScreenTab"));
			ScreenTab->SetDataDisplayText(SetDataText(TEXT("显示")));

			VideoTab->AddChildData(ScreenTab);

			UYCNListData_String* DisplayMode = NewObject<UYCNListData_String>();
			if (DisplayMode)
			{
				DisplayMode->SetDataID(FName("DisplayMode"));
				DisplayMode->SetDataDisplayText(SetDataText(TEXT("显示模式")));
				DisplayMode->SetDataDescription(SetDataText(TEXT("调整游戏的显示模式。\n\n无边框模式：以无边框窗口填满屏幕。\n\n窗口模式：以普通窗口显示游戏。")));
				DisplayMode->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("无边框模式")));
				DisplayMode->AddDataToOptionsList(SetDataText(TEXT("无边框模式")));
				DisplayMode->AddDataToOptionsList(SetDataText(TEXT("窗口模式")));
				BindCachedValue(DisplayMode, &UYCNGameUserSettings::GetCahcedDisplayMode, &UYCNGameUserSettings::SetCachedDisplayMode);
				ScreenTab->AddChildData(DisplayMode);
			}
			UYCNListData_String* ScreenRatio = NewObject<UYCNListData_String>();
			if (ScreenRatio)
			{
				ScreenRatio->SetDataID(FName("ScreenRatio"));
				ScreenRatio->SetDataDisplayText(SetDataText(TEXT("屏幕宽高比")));
				ScreenRatio->SetDataDescription(SetDataText(TEXT("调整游戏画面的宽高比。\n\n自动：按显示器桌面分辨率显示。\n\n16:9：固定为 1920x1080。\n\n21:9：使用显示器支持的最高分辨率。")));
				ScreenRatio->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("自动")));
				ScreenRatio->AddDataToOptionsList(SetDataText(TEXT("自动")));
				ScreenRatio->AddDataToOptionsList(SetDataText(TEXT("16:9")));
				ScreenRatio->AddDataToOptionsList(SetDataText(TEXT("21:9")));
				//绑定值
				BindCachedValue(ScreenRatio, &UYCNGameUserSettings::GetCachedScreenRatio, &UYCNGameUserSettings::SetCachedScreenRatio);
				ScreenTab->AddChildData(ScreenRatio);
			}
			UYCNListData_String* ScreenResolution = NewObject<UYCNListData_String>();
			if (ScreenResolution)
			{
				ScreenResolution->SetDataID(FName("ScreenResolution"));
				ScreenResolution->SetDataDisplayText(SetDataText(TEXT("屏幕分辨率")));
				ScreenResolution->SetDataDescription(SetDataText(TEXT("调整游戏的屏幕分辨率。")));
				ScreenResolution->GetSettingBinding().SetOwningDefaultValue(GetMaxResolutionText());
				ScreenResolution->AddDataToOptionsList(GetMaxResolutionText());
				ScreenResolution->AddDataToOptionsList(SetDataText(TEXT("1920x1080")));
				ScreenResolution->AddDataToOptionsList(SetDataText(TEXT("1600x900")));
				ScreenResolution->AddDataToOptionsList(SetDataText(TEXT("1280x700")));
				//绑定值
				BindCachedValue(ScreenResolution, &UYCNGameUserSettings::GetCachedScreenResolution, &UYCNGameUserSettings::SetCachedScreenResolution);
				ScreenTab->AddChildData(ScreenResolution);
			}
		}
	}
}

void UYCNListData_RegisterData::InitControlList()
{
	UYCNListData_Tab* ControlTab = NewObject<UYCNListData_Tab>();
	if (ControlTab)
	{
		ControlTab->SetDataID(FName("ControlTab"));
		ControlTab->SetDataDisplayText(SetDataText(TEXT("控制")));

		AvailableDataList.Add(ControlTab);
	}
}

void UYCNListData_RegisterData::InitAssistList()
{
	UYCNListData_Tab* AssistTab = NewObject<UYCNListData_Tab>();
	if (AssistTab)
	{
		AssistTab->SetDataID(FName("AssistTab"));
		AssistTab->SetDataDisplayText(SetDataText(TEXT("辅助")));

		AvailableDataList.Add(AssistTab);
	}
}

FText UYCNListData_RegisterData::SetDataText(const FString& InDaraString)
{
	return FText::FromString(InDaraString);
}

FText UYCNListData_RegisterData::GetMaxResolutionText()
{
	if (UserSettings.IsValid())
	{
		return UserSettings->GetMaxResolutionText();
	}
	return FText();
}
