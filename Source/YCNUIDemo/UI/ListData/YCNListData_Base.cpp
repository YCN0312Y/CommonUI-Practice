// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"

void UYCNListData_Base::AddChildData(UYCNListData_Base* InChildData)
{
	if (!IsValid(InChildData)) return;

	InChildData->SetDataParent(this);
	ChildDataList.Add(InChildData);
}

UYCNListData_Base* UYCNListData_Base::GetDataByChildDataID(const FName& InDataID)
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
