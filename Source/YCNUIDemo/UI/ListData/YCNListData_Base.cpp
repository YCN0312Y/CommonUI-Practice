// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"

void UYCNListData_Base::SetSettingTextBinding(FOnReadSettingValue InReadDelegate, FOnWriteSettingValue InWriteDelegate)
{
    //将参数绑定的委托给成员代理
	OnReadSettingValue = MoveTemp(InReadDelegate);
	OnWriteSettingValue = MoveTemp(InWriteDelegate);
}

bool UYCNListData_Base::TryReadSettingText(FText& OutValue) const
{
    if (OnReadSettingValue.IsBound())
    {
        //将代理获取来的值复制给传进来的变量
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
