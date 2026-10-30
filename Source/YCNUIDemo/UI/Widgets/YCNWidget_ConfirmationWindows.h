// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNUIDemo/Msic/YCNDataType.h"
#include "YCNWidget_ConfirmationWindows.generated.h"

class UCommonTextBlock;
class UDynamicEntryBox;

//按钮信息
struct FWindowButtonInfo
{
	//显示名称
	FText ButtonDisplayText;
	//按钮类型
	EConfirmationWindowsButtonType ButtonType;
};

UCLASS()
class YCNUIDEMO_API UYCNWindowInfo : public UObject
{
	GENERATED_BODY()

public:
	//窗口消息文本
	FText WindowMessageText;
	//按钮列表
	TArray<FWindowButtonInfo>ButtonArray;

public:
	static UYCNWindowInfo* CreateOKButton(const FText& InWindowMessageText);
	static UYCNWindowInfo* CreateYesNoButton(const FText& InWindowMessageText);
};

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_ConfirmationWindows : public UYCNWidget_ActivatableWidgetBase
{
	GENERATED_BODY()

private:
	//窗口消息
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>CommonText_WindowMessageText;
	//按钮
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox>EntryBox_Button;

public:
	void ButtonClicked(UYCNWindowInfo* InWindowInfo, TFunction<void(EConfirmationWindowsButtonType)> ButtonClickCallback);
};
