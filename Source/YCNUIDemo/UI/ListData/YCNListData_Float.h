// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Msic/YCNFunctionHelper.h"
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
				&& !FMath::IsNearlyEqual(CurrentValue, GetOwningDefaultValue(), KINDA_SMALL_NUMBER);
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

	//最小值
	float MinValue;

	//最大值
	float MaxValue;

public:
	//能否重置为默认值
	virtual bool CanResetToDefault()const override;

	//重置为默认值
	virtual bool ResetToDefault()const override;

	MAKE_SET_GET(float, MinValue);

	MAKE_SET_GET(float, MaxValue);

};
