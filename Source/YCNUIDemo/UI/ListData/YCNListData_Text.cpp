// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Text.h"

void UYCNListData_Text::SetClickCallBack(TFunction<void(EConfirmationWindowsButtonType)> InClickCallBack)
{
	ClickCallBack = InClickCallBack;
}
