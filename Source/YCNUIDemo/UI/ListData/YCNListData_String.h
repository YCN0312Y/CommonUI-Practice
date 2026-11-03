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
	struct FSettingBinding : TYCNSettingBinding<FText>
	{

	public:
		//能否重置为默认值
		virtual bool CanResetToDefault()const override
		{
			FText CurrentValue;

			return HasDefaultValue()
				&& HasWriteBinding()
				&& TryRead(CurrentValue)
				&& !CurrentValue.EqualTo(GetOwningDefaultValue());
		}
	};

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

protected:
	//设置绑定
	FSettingBinding SettingBinding;

	//选项列表
	UPROPERTY(Transient)
	TArray<FText>OptionList;

public:
	//能否重置为默认值
	virtual bool CanResetToDefault()const override;

	//重置为默认值
	virtual bool ResetToDefault()const override;

	//添加数据到选项列表
	void AddDataToOptionsList(const FText& InOptionText);

	//获取选项列表
	const TArray<FText>& GetOptionList()const { return OptionList; }
};

UCLASS()
class YCNUIDEMO_API UYCNListData_StringSwitch : public UYCNListData_String
{
	GENERATED_BODY()

};

UCLASS()
class YCNUIDEMO_API UYCNListData_StringBool : public UYCNListData_String
{
	GENERATED_BODY()

};
