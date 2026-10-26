// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"

void UYCNListData_RegisterData::InitDataList()
{
	AvailableDataList.Reset();

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
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("简单")));
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("正常")));
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("困难")));
				GameDifficulty->AddDataToOptionsList(SetDataText(TEXT("极难")));
				GameplayTab->AddChildData(GameDifficulty);
			}

			UYCNListData_Value* Test = NewObject<UYCNListData_Value>();
			if (Test)
			{
				Test->SetDataID(FName("Test"));
				Test->SetDataDisplayText(SetDataText(TEXT("测试标签")));
				Test->AddDataToOptionsList(SetDataText(TEXT("简单")));
				Test->AddDataToOptionsList(SetDataText(TEXT("正常")));
				Test->AddDataToOptionsList(SetDataText(TEXT("困难")));
				Test->AddDataToOptionsList(SetDataText(TEXT("极难")));
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
