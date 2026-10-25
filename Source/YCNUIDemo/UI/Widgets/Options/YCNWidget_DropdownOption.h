// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNWidget_DropdownOption.generated.h"

class UYCNWidget_ButtonBase;
class UScrollBox;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_DropdownOption : public UYCNWidget_ActivatableWidgetBase
{
	GENERATED_BODY()

private:
	//上一个
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_Previous;
	//选项列表
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox>ScrollBox_OptionList;
	//下一个
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_Next;
};
