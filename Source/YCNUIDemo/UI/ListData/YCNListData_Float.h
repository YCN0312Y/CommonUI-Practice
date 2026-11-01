// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNListData_Float.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNListData_Float : public UYCNListData_Value
{
	GENERATED_BODY()

public:
	struct FSettingBinding : TYCNSettingBinding<float>
	{

	public:
		//能否重置为默认值
		virtual bool CanResetToDefault()const override
		{
			float CurrentValue;

			return HasDefaultValue()
				&& HasWriteBinding()
				&& TryRead(CurrentValue)
				&& CurrentValue != GetOwningDefaultValue();
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

private:
	//设置绑定
	FSettingBinding SettingBinding;

public:
	//能否重置为默认值
	virtual bool CanResetToDefault()const override;

	//重置为默认值
	virtual bool ResetToDefault()const override;

};
