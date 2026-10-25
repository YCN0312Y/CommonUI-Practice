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

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_EntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject)override;
	//鼠标进入控件
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//鼠标离开控件
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

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

private:
	//高亮
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UBorder>Border_HighLight;
	//显示文本
	UPROPERTY(BlueprintReadOnly, Category = "YCN Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>CommonText_DisplayText;


};
