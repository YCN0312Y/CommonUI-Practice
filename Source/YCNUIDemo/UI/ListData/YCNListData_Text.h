// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Msic/YCNDataType.h"
#include "YCNListData_Text.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNListData_Text : public UYCNListData_Value
{
	GENERATED_BODY()
	
private:
	//点击数据弹窗类型
	UPROPERTY(Transient)
	EConfirmationWindowsType ClickedWindowsType;

	//弹窗信息
	UPROPERTY(Transient)
	FText WindowsMessageText;

	TFunction<void(EConfirmationWindowsButtonType)>ClickCallBack;

public:
	MAKE_SET_GET(EConfirmationWindowsType, ClickedWindowsType);
	MAKE_SET_GET(FText, WindowsMessageText);

	void SetClickCallBack(TFunction<void(EConfirmationWindowsButtonType)>InClickCallBack);
	TFunction<void(EConfirmationWindowsButtonType)>GetClickCallBack() { return ClickCallBack; }
};
