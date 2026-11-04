// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_String.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Float.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Text.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "GenericPlatform/GenericWindow.h"

void UYCNListData_RegisterData::InitDataList()
{
	AvailableDataList.Reset();

	if (!UserSettings.IsValid())
	{
		UserSettings = UYCNGameUserSettings::Get();
	}

	InitControllerList();
	InitKeyboardMouseList();
	InitGameSettingsList();
	InitDisplayList();
	InitHDList();
	InitAudioList();
	InitAssistList();
	InitQuitList();
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

void UYCNListData_RegisterData::InitControllerList()
{
	UYCNListData_Tab* ControllerTab = NewObject<UYCNListData_Tab>();
	if (ControllerTab)
	{
		ControllerTab->SetDataID(FName("ControllerTab"));
		ControllerTab->SetDataDisplayText(SetDataText(TEXT("控制器")));

		AvailableDataList.Add(ControllerTab);
	}
}

void UYCNListData_RegisterData::InitKeyboardMouseList()
{
	UYCNListData_Tab* KeyboardMouseTab = NewObject<UYCNListData_Tab>();
	if (KeyboardMouseTab)
	{
		KeyboardMouseTab->SetDataID(FName("KeyboardMouseTab"));
		KeyboardMouseTab->SetDataDisplayText(SetDataText(TEXT("键盘与鼠标")));

		AvailableDataList.Add(KeyboardMouseTab);
	}
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
					GlobalVolume->SetMaxValue(100.f);
					GlobalVolume->SetMinValue(0.f);
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
					MusicVolume->SetMaxValue(100.f);
					MusicVolume->SetMinValue(0.f);
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
					SFXVolume->SetMaxValue(100.f);
					SFXVolume->SetMinValue(0.f);
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
					VoiceVolume->SetMaxValue(100.f);
					VoiceVolume->SetMinValue(0.f);
					VoiceVolume->GetSettingBinding().SetOwningDefaultValue(100.f);
					//绑定值
					BindCachedValue(VoiceVolume, &UYCNGameUserSettings::GetCachedVoiceVolume, &UYCNGameUserSettings::SetCachedVoiceVolume);
					Volume->AddChildData(VoiceVolume);
				}
			}
		}
	}
}

void UYCNListData_RegisterData::InitDisplayList()
{
	UYCNListData_Tab* DisplayTab = NewObject<UYCNListData_Tab>();
	if (DisplayTab)
	{
		DisplayTab->SetDataID(FName("DisplayTab"));
		DisplayTab->SetDataDisplayText(SetDataText(TEXT("显示")));

		AvailableDataList.Add(DisplayTab);

		UYCNListData_Tab* ScreenTab = NewObject<UYCNListData_Tab>();
		if (ScreenTab)
		{
			ScreenTab->SetDataID(FName("ScreenTab"));
			ScreenTab->SetDataDisplayText(SetDataText(TEXT("显示")));

			DisplayTab->AddChildData(ScreenTab);

			UYCNListData_String* DisplayMode = NewObject<UYCNListData_String>();
			if (DisplayMode)
			{
				DisplayMode->SetDataID(FName("DisplayMode"));
				DisplayMode->SetDataDisplayText(SetDataText(TEXT("显示模式")));
				DisplayMode->SetDataDescription(SetDataText(TEXT("调整游戏的显示模式。\n\n无边框模式：以无边框窗口填满屏幕。\n\n窗口模式：以普通窗口显示游戏。")));
				DisplayMode->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("无边框模式")));
				DisplayMode->AddDataToOptionsList(SetDataText(TEXT("无边框模式")));
				DisplayMode->AddDataToOptionsList(SetDataText(TEXT("窗口模式")));
				//绑定值
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
				//将可用的分辨率添加进选项列表
				AppendResolutionList(ScreenResolution);
				ScreenResolution->GetSettingBinding().SetOwningDefaultValue(GetMaxResolutionText());
				//绑定值
				BindCachedValue(ScreenResolution, &UYCNGameUserSettings::GetCachedScreenResolution, &UYCNGameUserSettings::SetCachedScreenResolution);
				ScreenTab->AddChildData(ScreenResolution);
			}
			UYCNListData_StringSwitch* FrameRateLimit = NewObject<UYCNListData_StringSwitch>();
			if (FrameRateLimit)
			{
				FrameRateLimit->SetDataID(FName("FrameRateLimit"));
				FrameRateLimit->SetDataDisplayText(SetDataText(TEXT("帧率上限")));
				FrameRateLimit->SetDataDescription(SetDataText(TEXT("调整游戏的帧率。")));
				FrameRateLimit->AddDataToOptionsList(SetDataText(TEXT("关闭")));
				FrameRateLimit->AddDataToOptionsList(SetDataText(TEXT("30")));
				FrameRateLimit->AddDataToOptionsList(SetDataText(TEXT("60")));
				FrameRateLimit->AddDataToOptionsList(SetDataText(TEXT("90")));
				FrameRateLimit->AddDataToOptionsList(SetDataText(TEXT("120")));
				FrameRateLimit->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("关闭")));
				BindCachedValue(FrameRateLimit, &UYCNGameUserSettings::GetCachedFrameRateLimit, &UYCNGameUserSettings::SetCachedFrameRateLimit);
				ScreenTab->AddChildData(FrameRateLimit);
			}
			UYCNListData_StringBool* VerticalSync = NewObject<UYCNListData_StringBool>();
			if (VerticalSync)
			{
				VerticalSync->SetDataID(FName("VerticalSync"));
				VerticalSync->SetDataDisplayText(SetDataText(TEXT("垂直同步")));
				VerticalSync->SetDataDescription(SetDataText(TEXT("调整游戏的垂直同步。")));
				VerticalSync->AddDataToOptionsList(SetDataText(TEXT("关闭")));
				VerticalSync->AddDataToOptionsList(SetDataText(TEXT("开启")));
				VerticalSync->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("关闭")));
				BindCachedValue(VerticalSync, &UYCNGameUserSettings::GetVerticalSync, &UYCNGameUserSettings::SetVerticalSync);
				ScreenTab->AddChildData(VerticalSync);
			}
		}
		UYCNListData_Tab* ScreenEffect = NewObject<UYCNListData_Tab>();
		if (ScreenEffect)
		{
			ScreenEffect->SetDataID(FName("ScreenEffect"));
			ScreenEffect->SetDataDisplayText(SetDataText(TEXT("显示效果")));

			DisplayTab->AddChildData(ScreenEffect);

			UYCNListData_StringSwitch* DynamicFuzzy = NewObject<UYCNListData_StringSwitch>();
			if (DynamicFuzzy)
			{
				DynamicFuzzy->SetDataID(FName("DynamicFuzzy"));
				DynamicFuzzy->SetDataDisplayText(SetDataText(TEXT("动态模糊")));
				DynamicFuzzy->SetDataDescription(SetDataText(TEXT("调整游戏的动态模糊。")));
				DynamicFuzzy->AddDataToOptionsList(SetDataText(TEXT("自动")));
				DynamicFuzzy->AddDataToOptionsList(SetDataText(TEXT("强")));
				DynamicFuzzy->AddDataToOptionsList(SetDataText(TEXT("弱")));
				DynamicFuzzy->AddDataToOptionsList(SetDataText(TEXT("关闭")));
				DynamicFuzzy->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("自动")));
				BindCachedValue(DynamicFuzzy, &UYCNGameUserSettings::GetCachedDynamicFuzzy, &UYCNGameUserSettings::SetCachedDynamicFuzzy);

				ScreenEffect->AddChildData(DynamicFuzzy);
			}

			UYCNListData_Float* SharpenAmount = NewObject<UYCNListData_Float>();
			if (SharpenAmount)
			{
				SharpenAmount->SetDataID(FName("SharpenAmount"));
				SharpenAmount->SetDataDisplayText(SetDataText(TEXT("锐化强度")));
				SharpenAmount->SetDataDescription(SetDataText(TEXT("调整游戏的锐化强度。")));
				SharpenAmount->SetMinValue(0.f);
				SharpenAmount->SetMaxValue(10.f);
				SharpenAmount->GetSettingBinding().SetOwningDefaultValue(5.f);
				BindCachedValue(SharpenAmount, &UYCNGameUserSettings::GetCachedSharpenAmount, &UYCNGameUserSettings::SetCachedSharpenAmount);

				ScreenEffect->AddChildData(SharpenAmount);
			}
			
		}
	}
}

void UYCNListData_RegisterData::InitHDList()
{
	UYCNListData_Tab* HDTab = NewObject<UYCNListData_Tab>();
	if (HDTab)
	{
		HDTab->SetDataID(FName("HDTab"));
		HDTab->SetDataDisplayText(SetDataText(TEXT("画质")));

		AvailableDataList.Add(HDTab);

		UYCNListData_Tab* HDSetting = NewObject<UYCNListData_Tab>();
		if (HDSetting)
		{
			HDSetting->SetDataID(FName("HDSetting"));
			HDSetting->SetDataDisplayText(SetDataText(TEXT("画质设置")));

			HDTab->AddChildData(HDSetting);

			UYCNListData_Text* ApplyHD = NewObject<UYCNListData_Text>();
			if (ApplyHD)
			{
				ApplyHD->SetDataID(FName("ApplyHD"));
				ApplyHD->SetDataDisplayText(SetDataText(TEXT("应用画质等级")));
				ApplyHD->SetDataDescription(SetDataText(TEXT("设定游戏画面质量。")));

				HDSetting->AddChildData(ApplyHD);
			}
		}
		UYCNListData_Tab* SRSampling = NewObject<UYCNListData_Tab>();
		if (SRSampling)
		{
			SRSampling->SetDataID(FName("SRSampling"));
			SRSampling->SetDataDisplayText(SetDataText(TEXT("超分辨率采样")));

			HDTab->AddChildData(SRSampling);

			UYCNListData_Float* SRSharpness = NewObject<UYCNListData_Float>();
			if (SRSharpness)
			{
				SRSharpness->SetDataID(FName("SRSharpness"));
				SRSharpness->SetDataDisplayText(SetDataText(TEXT("超采样清晰度")));
				SRSharpness->SetMinValue(25.f);
				SRSharpness->SetMaxValue(100.f);
				SRSharpness->GetSettingBinding().SetOwningDefaultValue(58.f);
				BindCachedValue(SRSharpness, &UYCNGameUserSettings::GetRenderResolutionPercent, &UYCNGameUserSettings::SetRenderResolutionPercent);
				SRSampling->AddChildData(SRSharpness);
			}

			UYCNListData_StringSwitch* AAMethod = NewObject<UYCNListData_StringSwitch>();
			if (AAMethod)
			{
				AAMethod->SetDataID(FName("AAMethod"));
				AAMethod->SetDataDisplayText(SetDataText(TEXT("超分辨率采样")));
				AAMethod->AddDataToOptionsList(SetDataText(TEXT("FXAA")));
				AAMethod->AddDataToOptionsList(SetDataText(TEXT("TAA")));
				AAMethod->AddDataToOptionsList(SetDataText(TEXT("MSAA")));
				AAMethod->AddDataToOptionsList(SetDataText(TEXT("TSR")));
				AAMethod->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("TSR")));
				BindCachedValue(AAMethod, &UYCNGameUserSettings::GetCachedAAMethod, &UYCNGameUserSettings::SetCachedAAMethod);
				SRSampling->AddChildData(AAMethod);
			}

			UYCNListData_StringSwitch* FrameGeneration = NewObject<UYCNListData_StringSwitch>();
			if (FrameGeneration)
			{
				FrameGeneration->SetDataID(FName("FrameGeneration"));
				FrameGeneration->SetDataDisplayText(SetDataText(TEXT("帧生成")));
				FrameGeneration->AddDataToOptionsList(SetDataText(TEXT("1X")));
				FrameGeneration->AddDataToOptionsList(SetDataText(TEXT("2X")));
				FrameGeneration->AddDataToOptionsList(SetDataText(TEXT("4X")));
				FrameGeneration->AddDataToOptionsList(SetDataText(TEXT("8X")));
				FrameGeneration->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("4X")));
				BindCachedValue(FrameGeneration, &UYCNGameUserSettings::GetFrameGeneration, &UYCNGameUserSettings::SetFrameGeneration);
				SRSampling->AddChildData(FrameGeneration);
			}

		}
		UYCNListData_Tab* RayTracing = NewObject<UYCNListData_Tab>();
		if (RayTracing)
		{
			RayTracing->SetDataID(FName("RayTracing"));
			RayTracing->SetDataDisplayText(SetDataText(TEXT("全景光线追踪")));

			HDTab->AddChildData(RayTracing);
		}
	}
}

void UYCNListData_RegisterData::InitAssistList()
{
	UYCNListData_Tab* AssistTab = NewObject<UYCNListData_Tab>();
	if (AssistTab)
	{
		AssistTab->SetDataID(FName("AssistTab"));
		AssistTab->SetDataDisplayText(SetDataText(TEXT("辅助设置")));

		AvailableDataList.Add(AssistTab);

		UYCNListData_StringBool* Subtitle = NewObject<UYCNListData_StringBool>();
		if (Subtitle)
		{
			Subtitle->SetDataID(FName("Subtitle"));
			Subtitle->SetDataDisplayText(SetDataText(TEXT("字幕")));
			Subtitle->SetDataDescription(SetDataText(TEXT("设置是否显示字幕。")));
			Subtitle->AddDataToOptionsList(SetDataText(TEXT("关闭")));
			Subtitle->AddDataToOptionsList(SetDataText(TEXT("开启")));
			Subtitle->GetSettingBinding().SetOwningDefaultValue(SetDataText(TEXT("开启")));
			BindCachedValue(Subtitle, &UYCNGameUserSettings::GetbSubtitle, &UYCNGameUserSettings::SetbSubtitle);
			AssistTab->AddChildData(Subtitle);
		}

	}
}

void UYCNListData_RegisterData::InitQuitList()
{
	UYCNListData_Tab* QuitTab = NewObject<UYCNListData_Tab>();
	if (QuitTab)
	{
		QuitTab->SetDataID(FName("QuitTab"));
		QuitTab->SetDataDisplayText(SetDataText(TEXT("退出游戏")));

		AvailableDataList.Add(QuitTab);

		UYCNListData_Text* QuitGame = NewObject<UYCNListData_Text>();
		if (QuitGame)
		{
			QuitGame->SetDataID(FName("QuitGame"));
			QuitGame->SetDataDisplayText(SetDataText(TEXT("退出至桌面")));
			QuitGame->SetDataDescription(SetDataText(TEXT("退出游戏到桌面。")));

			QuitTab->AddChildData(QuitGame);
		}
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

//将可用的分辨率添加进选项列表
void UYCNListData_RegisterData::AppendResolutionList(UYCNListData_String* InData)
{
	if (UserSettings.IsValid())
	{
		for (const FIntPoint& AddedOption : UserSettings->GetAvailableResolution())
		{
			InData->AddDataToOptionsList(UYCNGameUserSettings::GetResolutionText(AddedOption));
		}
	}
}
