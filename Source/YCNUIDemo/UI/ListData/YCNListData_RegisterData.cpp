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

		AvailableDataList.Add(GameplayTab);

		//游戏难度
		{
			UYCNListData_Value* GameDifficulty = NewObject<UYCNListData_Value>();
			if (GameDifficulty)
			{
				GameDifficulty->SetDataID(FName("GameDifficulty"));
				GameDifficulty->SetDataDisplayText(SetDataText(TEXT("游戏难度")));

				GameplayTab->AddChildData(GameDifficulty);
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

		AvailableDataList.Add(AudioTab);
	}
}

void UYCNListData_RegisterData::InitVideoList()
{
	UYCNListData_Tab* VideoTab = NewObject<UYCNListData_Tab>();
	if (VideoTab)
	{
		VideoTab->SetDataID(FName("VideoTab"));

		AvailableDataList.Add(VideoTab);
	}
}

void UYCNListData_RegisterData::InitControlList()
{
	UYCNListData_Tab* ControlTab = NewObject<UYCNListData_Tab>();
	if (ControlTab)
	{
		ControlTab->SetDataID(FName("ControlTab"));

		AvailableDataList.Add(ControlTab);
	}
}

void UYCNListData_RegisterData::InitAssistList()
{
	UYCNListData_Tab* AssistTab = NewObject<UYCNListData_Tab>();
	if (AssistTab)
	{
		AssistTab->SetDataID(FName("AssistTab"));

		AvailableDataList.Add(AssistTab);
	}
}

FText UYCNListData_RegisterData::SetDataText(const FString& InDaraString)
{
	return FText::FromString(InDaraString);
}
