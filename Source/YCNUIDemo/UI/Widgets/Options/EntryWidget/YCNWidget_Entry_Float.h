// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNWidget_Entry_Float.generated.h"

class UCommonTextBlock;
class USlider;
class UYCNListData_Float;
class USoundBase;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Entry_Float : public UYCNWidget_EntryBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;

	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	//切换条目状态
	virtual void SwitchItemStatus(bool bIsSwitch)override;

public:
	//刷新条目数据
	virtual void RefreshEntryData()override;

	virtual bool IsValueModified() const override;

private:
	//当前音量
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>CommonText_CurrentVolume;
	//滑块
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USlider>Slider_VolumeSlider;
	//最小音量
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>CommonText_MinVolume;
	//最大音量
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>CommonText_MaxVolume;

	//持有的值数据
	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_Float>OwningValueData;

	//滑块值更新音效
	UPROPERTY(EditDefaultsOnly, Category = "YCN Widget|Style")
	TObjectPtr<USoundBase>SliderValueChangedSound;

	//滑块默认颜色
	UPROPERTY(EditDefaultsOnly, Category = "YCN Widget|Style")
	FColor SliderDefaultColor;

	//滑块选择颜色
	UPROPERTY(EditDefaultsOnly, Category = "YCN Widget|Sound")
	FColor SliderSelectedColor;

	float CurrentValueVerticalOffset = -2.f;

	//刷新条目
	bool bRefreshingEntry = false;

private:
	UFUNCTION()
	void NotifyUpdateCurrentVolume(float InValue);

};
