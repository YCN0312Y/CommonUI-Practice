// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCNListData_RegisterData.generated.h"

class UYCNListData_Tab;

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_RegisterData : public UObject
{
	GENERATED_BODY()

public:
	//初始化数据列表
	void InitDataList();

	UYCNListData_Tab* GetDataByDataID(const FName& InDataID);

private:
	UPROPERTY(Transient)
	TArray<UYCNListData_Tab*> AvailableDataList;

private:
	//初始化游戏设置
	void InitGameSettingsList();
	//初始化音频设置
	void InitAudioList();
	//初始化视频设置
	void InitVideoList();
	//初始化控制设置
	void InitControlList();
	//初始化帮助设置
	void InitAssistList();

	//设置数据字符串
	FText SetDataText(const FString& InDaraString);

public:
	const TArray<UYCNListData_Tab*>& GetAvailableDataList() const { return AvailableDataList; }
};
