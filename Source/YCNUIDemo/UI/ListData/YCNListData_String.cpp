// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_String.h"

bool UYCNListData_String::CanResetToDefault() const
{
	return SettingBinding.CanResetToDefault();
}

bool UYCNListData_String::ResetToDefault() const
{
	return SettingBinding.ResetToDefault();
}

void UYCNListData_String::AddDataToOptionsList(const FText& InOptionText)
{
	OptionList.Add(InOptionText);
}
