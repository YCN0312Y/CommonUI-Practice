// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNListData_RegisterKeyMouseData.generated.h"

class UYCNListData_Base;
class UYCNListData_Tab;
class UYCNListData_Image;

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_RegisterKeyMouseData : public UYCNListData_RegisterData
{
	GENERATED_BODY()

public:
	//初始化数据列表
	void InitDataList()override;

	UYCNListData_Tab* GetDataByDataID(const FName& InDataID)override;

private:
	UPROPERTY(Transient)
	TArray<UYCNListData_Tab*> KeyboardMouseDataList;

private:
	//初始化键盘数据列表
	void InitKeyboardList();

	//初始化鼠标数据列表
	void InitMouseList();

	//创建图标式标签
	void CreateIconTabData(UYCNListData_Tab* InData, const FName& InDataID, UYCNListData_Base* InParentData = nullptr);

public:
	const TArray<UYCNListData_Tab*>& GetKeyboardMouseDataList()const { return KeyboardMouseDataList; }
};
