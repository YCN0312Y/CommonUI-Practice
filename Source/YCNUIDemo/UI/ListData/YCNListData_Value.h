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

	//默认值
	UPROPERTY(Transient)
	FText OwningDefaultValue;
public:
	//添加数据到选项列表
	void AddDataToOptionsList(const FText& InOption);

	//设置数据默认值
	void SetDataDefaultValue(const FText& InValue);

	//获取持有的默认值
	const FText& GetOwningDefaultValue()const;

	//获取选项列表
	const TArray<FText>& GetOptionList()const { return OptionList; }
};
