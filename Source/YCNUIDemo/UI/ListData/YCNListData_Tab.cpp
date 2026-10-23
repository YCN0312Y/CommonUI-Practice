// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"

void UYCNListData_Tab::InitializationData()
{

}

void UYCNListData_Tab::AddChildData(UYCNListData_Base* InChildData)
{
	ChildDataList.Add(InChildData);
}

UYCNListData_Base* UYCNListData_Tab::GetDataByChildDataID(const FName& InDataID)
{
	for (const TObjectPtr<UYCNListData_Base>& Data : ChildDataList)
	{
		if (Data && Data->GetDataID() == InDataID)
		{
			return Data.Get();
		}
	}
	return nullptr;
}
