// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNListData_Value.generated.h"

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_Value : public UYCNListData_Base
{
	GENERATED_BODY()

private:
	//选项列表
	UPROPERTY(Transient)
	TArray<FText>OptionList;

public:
	//添加数据到选项列表
	void AddDataToOptionsList(const FText& InOption);

	const TArray<FText>& GetOptionList()const { return OptionList; }
};
