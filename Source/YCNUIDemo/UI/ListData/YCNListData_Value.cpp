// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"

void UYCNListData_Value::AddDataToOptionsList(const FName& InDataID, const FText& InOptionText)
{
	FValueDataList ValueDataList;
	ValueDataList.DataID = InDataID;
	ValueDataList.OptionText = InOptionText;

	OptionList.Add(ValueDataList);
}

void UYCNListData_Value::SetDataDefaultValue(const FText& InDefaultValue)
{
	if (OwningDefaultValue.IsEmpty())
	{
		OwningDefaultValue = InDefaultValue;
	}
}

const FText& UYCNListData_Value::GetOwningDefaultValue() const
{
	return OwningDefaultValue.IsEmpty() ? FText::GetEmpty() : OwningDefaultValue;
}