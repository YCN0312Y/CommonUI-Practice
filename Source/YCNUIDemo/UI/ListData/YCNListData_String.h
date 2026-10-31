// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNListData_String.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNListData_String : public UYCNListData_Value
{
	GENERATED_BODY()

public:
	using FSettingBinding = TYCNSettingBinding<FText>;

	//可写对象
	FSettingBinding& GetSettingBinding()
	{
		return SettingBinding;
	}
	//只读对象
	const FSettingBinding& GetSettingBinding() const
	{
		return SettingBinding;
	}

private:
	//设置绑定
	FSettingBinding SettingBinding;

	//选项列表
	UPROPERTY(Transient)
	TArray<FText>OptionList;

	//默认值
	UPROPERTY(Transient)
	FText OwningTextDefaultValue;

protected:
	//是否可以重置为默认值
	virtual bool CanResetToDefault()const override;

public:
	//重置为默认值
	virtual bool ResetToDefault()const override;

	//添加数据到选项列表
	void AddDataToOptionsList(const FText& InOptionText);

	//设置数据默认值
	void SetOwningTextDefaultValue(const FText& InDefaultValue);

	//获取数据默认值
	const FText& GetOwningTextDefaultValue()const;

	//获取选项列表
	const TArray<FText>& GetOptionList()const { return OptionList; }
};
