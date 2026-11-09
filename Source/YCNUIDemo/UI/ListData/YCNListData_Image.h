// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Msic/YCNDataType.h"
#include "YCNListData_Image.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNListData_Image : public UYCNListData_Value
{
	GENERATED_BODY()

public:
	struct FSettingBinding : TYCNSettingBinding<FKey>
	{
		virtual bool CanResetToDefault() const override
		{
			FKey CurrentValue;

			return HasDefaultValue()
				&& HasWriteBinding()
				&& TryRead(CurrentValue)
				&& CurrentValue != GetOwningDefaultValue();
		}
	};

private:
	TFunction<void()>ClickCallBack;

	//预览键
	UPROPERTY(Transient)
	FKey PreviewKey;

	//当前条目是否正在等待键鼠输入。
	UPROPERTY(Transient)
	bool bWaitingForInput = false;

	UPROPERTY(Transient)
	FKey DefaultPreviewKey;

	FSettingBinding SettingBinding;

public:
	//设置点击回调
	void SetClickCallBack(TFunction<void()>InClickCallBack);

	//获取点击回调
	TFunction<void()>GetClickCallBack() { return ClickCallBack; }

	//预览键是否有效
	bool HasPreviewKey() const;

	//设置预览键
	void SetPreviewKey(const FKey& InKey);

	//获取预览键
	const FKey& GetPreviewKey() const { return PreviewKey; }

	//是否正在等待键鼠输入
	bool IsWaitingForInput() const { return bWaitingForInput; }

	//设置是否正在等待输入
	void SetWaitingForInput(bool bInWaitingForInput);

	FSettingBinding& GetSettingBinding()
	{
		return SettingBinding;
	}

	const FSettingBinding& GetSettingBinding() const
	{
		return SettingBinding;
	}

	//判断能否重置为默认值
	virtual bool CanResetToDefault() const override;

	//重置为默认值
	virtual bool ResetToDefault() const override;

};
