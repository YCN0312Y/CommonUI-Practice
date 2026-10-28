// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "YCNWidget_EntryBase.generated.h"

class UCommonTextBlock;
class UBorder;
class USoundBase;
class UCommonTextStyle;
class UYCNGameUserSettings;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_EntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	/*
	* IUserObjectListEntry Begin-----
	*/
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;
	//选择小条目
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	virtual void NativeOnEntryReleased() override;
	/*
	* IUserObjectListEntry End-----
	*/

	//聚焦
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	//鼠标进入时
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;

	//切换条目状态
	UFUNCTION(BlueprintCallable, Category = "YCN Widget")
	virtual void SwitchItemStatus(bool bIsSwitch);

	void SetEnrtyDisplayText(const FText& InDisplayText);

protected:
	//音效
	UPROPERTY(EditDefaultsOnly, Category = "YCN Widget|Sound")
	TObjectPtr<USoundBase> HoverSound;
	//字样式默认
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCN Widget|Text Style")
	TSubclassOf<UCommonTextStyle>TextStyle_Default;
	//字样式选择
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCN Widget|Text Style")
	TSubclassOf<UCommonTextStyle>TextStyle_Selected;

	//用户设置
	UPROPERTY(Transient)
	TWeakObjectPtr<UYCNGameUserSettings>UserSettings;

private:
	//高亮
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UBorder>Border_HighLight;
	//显示文本
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>CommonText_DisplayText;


};
