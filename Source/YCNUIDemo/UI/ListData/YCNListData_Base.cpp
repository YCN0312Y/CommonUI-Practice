// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"

void UYCNListData_Base::SetSettingTextBinding(FOnReadSettingValue InReadDelegate, FOnWriteSettingValue InWriteDelegate)
{
	OnReadSettingValue = MoveTemp(InReadDelegate);
	OnWriteSettingValue = MoveTemp(InWriteDelegate);
}

bool UYCNListData_Base::TryReadSettingText(FText& OutValue) const
{
    if (OnReadSettingValue.IsBound())
    {
        OutValue = OnReadSettingValue.Execute();
        return true;
    }

    return false;
}

bool UYCNListData_Base::TryWriteSettingText(const FText& InValue) const
{
    if (OnWriteSettingValue.IsBound())
    {
        OnWriteSettingValue.Execute(InValue);
        return true;
    }
	return false;
}
