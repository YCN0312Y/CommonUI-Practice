// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNWidget_Entry_Float.generated.h"

class UCommonTextBlock;
class USlider;
class UYCNListData_Float;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Entry_Float : public UYCNWidget_EntryBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;
	
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

};
