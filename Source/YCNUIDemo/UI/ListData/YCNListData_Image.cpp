// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"

void UYCNListData_Image::SetClickCallBack(TFunction<void()> InClickCallBack)
{
	ClickCallBack = MoveTemp(InClickCallBack);
}

bool UYCNListData_Image::HasPreviewKey() const
{
	return PreviewKey.IsValid();
}

void UYCNListData_Image::SetPreviewKey(const FKey& InKey)
{
	PreviewKey = InKey;
}

void UYCNListData_Image::SetWaitingForInput(bool bInWaitingForInput)
{
	bWaitingForInput = bInWaitingForInput;
}

bool UYCNListData_Image::CanResetToDefault() const
{
	return SettingBinding.CanResetToDefault();
}

bool UYCNListData_Image::ResetToDefault() const
{
	return SettingBinding.ResetToDefault();
}
