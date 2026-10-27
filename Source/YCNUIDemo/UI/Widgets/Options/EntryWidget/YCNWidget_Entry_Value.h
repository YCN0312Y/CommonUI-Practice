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

	//切换条目状态
	virtual void SwitchItemStatus(bool bIsSwitch)override;

	virtual void NativeOnEntryReleased() override;

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

	//当前由 MenuAnchor 创建并显示的菜单实例
	UPROPERTY(Transient)
	TWeakObjectPtr<UYCNWidget_DropdownOption> CurrentDropdownMenu;

public:
	//打开列表背景
	DECLARE_DELEGATE_OneParam(FOnDropdownOpenChanged, bool bIsOpen);
	FOnDropdownOpenChanged OnDropdownOpenChanged;

private:
	//点击下拉按钮
	void OnClicked_DropdownButton();

	//创建选项菜单内容
	UFUNCTION()
	UUserWidget* CreateOptionMenuContent();

	//菜单打开/关闭
	UFUNCTION()
	void OnOpen_MenuAnchorOption(bool bIsOpen);

	//点击下拉菜单中的按钮回调
	void OnClick_ListCreatedButton(FText InSelectedText);

public:
	//关闭下拉菜单
	void CloseDropdownMenu(bool bIsFocusThis = true);
};
