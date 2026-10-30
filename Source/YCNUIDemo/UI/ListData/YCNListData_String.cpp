// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_String.h"

bool UYCNListData_String::CanResetToDefault() const
{
	FText CurrentValue;
	   
	return HasDefaultValue() //检查是否有默认值
		&& HasWriteBinding() //检查写入代理是否绑定
		&& TryReadSettingText(CurrentValue) //将在设置上设置的值赋给当前值
		&& !CurrentValue.EqualTo(GetOwningDefaultValue());//当前值 != 默认值
}

bool UYCNListData_String::ResetToDefault() const
{
	//可以重置为默认值就重置为默认值
	return CanResetToDefault() && TryWriteSettingText(OwningDefaultValue);
}

void UYCNListData_String::AddDataToOptionsList(const FText& InOptionText)
{
	OptionList.Add(InOptionText);
}

void UYCNListData_String::SetOwningDefaultValue(const FText& InDefaultValue)
{
	if (OwningDefaultValue.IsEmpty())
	{
		OwningDefaultValue = InDefaultValue;
		bHasDefalutValue = true;
	}
}

const FText& UYCNListData_String::GetOwningDefaultValue() const
{
	return OwningDefaultValue.IsEmpty() ? FText::GetEmpty() : OwningDefaultValue;
}