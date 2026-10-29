// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"

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
			UYCNListData_Value* GameDifficulty = NewObject<UYCNListData_Value>();
			if (GameDifficulty)
			{
				GameDifficulty->SetDataID(FName("GameDifficulty"));
				GameDifficulty->SetDataDisplayText(SetDataText(TEXT("游戏难度")));
				GameDifficulty->AddDataToOptionsList(FName("Easy"), SetDataText(TEXT("简单")));
				GameDifficulty->AddDataToOptionsList(FName("Normal"), SetDataText(TEXT("正常")));
				GameDifficulty->AddDataToOptionsList(FName("Difficulty"), SetDataText(TEXT("困难")));
				GameDifficulty->AddDataToOptionsList(FName("Hardest"), SetDataText(TEXT("极难")));
				GameDifficulty->SetDataDefaultValue(SetDataText(TEXT("简单")));
				const FString Easy = TEXT("简单：专注于剧情与探索，敌人较弱且资源充沛，容错率极高，适合新手或轻松体验。\n\n");
				const FString Normal = TEXT("正常：制作组推荐的基准难度，攻防均衡，需要适度运用操作与策略，体验平滑充实。\n\n");
				const FString Difficulty = TEXT("困难：敌人更具侵略性，失误代价更高，考验核心机制掌握与战术应变。\n\n");
				const FString Hardest = TEXT("极难：极限试炼，容错率近乎为零，资源匮乏且敌人致命，专为追求极致挑战的硬核玩家准备。\n\n");
				GameDifficulty->SetDataDescription(SetDataText(Easy + Normal + Difficulty + Hardest));
				BindCachedValue(GameDifficulty,
					&UYCNGameUserSettings::GetCachedGameDifficultyText, &UYCNGameUserSettings::SetCachedGameDifficultyText);
				GameplayTab->AddChildData(GameDifficulty);
			}

			UYCNListData_Value* Test = NewObject<UYCNListData_Value>();
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

void UYCNListData_RegisterData::BindCachedValue(UYCNListData_Base* InData, FTextSettingGetter InGetter, FTextSettingSetter InSetter)
{
	if (!IsValid(InData) || !InGetter || !InSetter)return;

	if(UserSettings.IsValid())
	{
		InData->SetSettingTextBinding(
			FOnReadSettingValue::CreateLambda(
				[this, InGetter]()->FText
				{
					if (UYCNGameUserSettings* Settings = UserSettings.Get())
					{
						return (Settings->*InGetter)();
					}

					return FText::GetEmpty();
				}),
			FOnWriteSettingValue::CreateLambda(
				[this, InSetter](const FText& InValue)
				{
					if (UYCNGameUserSettings* Settings = UserSettings.Get())
					{
						(Settings->*InSetter)(InValue);

						// 保存设置。
						Settings->SaveSettings();

						OnSettingValueChanged.Execute();
					}

				})
		);
	}
}
