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

void UYCNListData_Base::AddCondition(TFunction<bool()> InCondition)
{
	EditConditions.Add(MoveTemp(InCondition));
}

bool UYCNListData_Base::CanEditListData()
{
    // 没有条件的条目，使用注册时设置的状态。
    if (EditConditions.IsEmpty())
    {
        return bDataDisabled;
    }

    bDataDisabled = true;

    for (const TFunction<bool()>& Condition : EditConditions)
    {
        if (!Condition())
        {
            bDataDisabled = false;
            break;
        }
    }

    return bDataDisabled;
}
