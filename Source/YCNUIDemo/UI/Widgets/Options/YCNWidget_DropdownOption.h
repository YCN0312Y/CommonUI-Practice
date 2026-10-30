// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNWidget_DropdownOption.generated.h"

class UYCNWidget_ButtonBase;
class UScrollBox;
class UYCNListData_String;
class USizeBox;

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
	//选项列表数据
	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_String>OptionListData;
	//选项列表按钮
	UPROPERTY(EditDefaultsOnly, Category = "YCN Widget")
	TSubclassOf<UYCNWidget_ButtonBase>OptionButton;

public:
	//更新选择文本代理
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateSelectedText, FText InOptionText);
	FOnUpdateSelectedText OnUpdateSelectedText;

public:
	//设置选项列表数据
	void SetOptionListData(UYCNListData_String* InData);
	//添加数据到选项列表
	void AddDataToOptionList();

	//点击创建的按钮
	void OnClick_CreatedButton(FText InOptionText);
};
