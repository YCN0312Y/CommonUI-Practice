// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Float.h"

bool UYCNListData_Float::CanResetToDefault() const
{
	return SettingBinding.CanResetToDefault();
}

bool UYCNListData_Float::ResetToDefault() const
{
	return SettingBinding.ResetToDefault();
}
