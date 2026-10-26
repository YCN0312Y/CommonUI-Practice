// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNWidget_Entry_Value.generated.h"

class UYCNWidget_ButtonBase;
class UCommonTextBlock;
class UYCNListData_Value;
class UYCNWidget_DropdownOption;
class UMenuAnchor;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Entry_Value : public UYCNWidget_EntryBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized()override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;

	//聚焦
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	virtual void SwitchItemStatus(bool bIsSwitch)override;;

private:
	/*
	* BindWidget--------------------
	*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMenuAnchor>MenuAnchor_Option;
	//选择文本
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>CommonText_SelectedText;
	//下拉按钮
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_DropdownButton;
	/*
	* BindWidget--------------------
	*/

	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_Value>OwningValueData;
	//下拉菜单类
	UPROPERTY(EditDefaultsOnly, Category = "YCN Widget")
	TSoftClassPtr<UYCNWidget_DropdownOption>DropdownOptionClass;

private:
	void OnClicked_DropdownButton();
	//创建内容
	UFUNCTION()
	UUserWidget* CreateOptionMenuContent();

	void OnDropdownOptionSelected(const FText& InOptionText);
};
