// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNWidget_Entry_Value.generated.h"

class UYCNWidget_ButtonBase;
class UYCNWidget_RotatorBase;
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Entry_Value : public UYCNWidget_EntryBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;

private:
	//上一个
	UPROPERTY(BlueprintReadOnly, Category = "YCN Entry Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_Previous;
	//选项列表
	UPROPERTY(BlueprintReadOnly, Category = "YCN Entry Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_RotatorBase>CommonRotator_OptionList;
	//下一个
	UPROPERTY(BlueprintReadOnly, Category = "YCN Entry Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_Next;
};
