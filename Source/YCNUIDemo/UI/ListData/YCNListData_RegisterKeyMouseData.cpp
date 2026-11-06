// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/ListData/YCNListData_RegisterKeyMouseData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"

void UYCNListData_RegisterKeyMouseData::InitDataList()
{
	InitKeyboardList();
	InitMouseList();
}

UYCNListData_Tab* UYCNListData_RegisterKeyMouseData::GetDataByDataID(const FName& InDataID)
{
	return nullptr;
}

void UYCNListData_RegisterKeyMouseData::InitKeyboardList()
{
	UYCNListData_Tab* KeyboardTab = NewObject<UYCNListData_Tab>();
	CreateIconTabData(KeyboardTab, FName("KeyboardTab"));
	{

	}
}

void UYCNListData_RegisterKeyMouseData::InitMouseList()
{
	UYCNListData_Tab* MouseTab = NewObject<UYCNListData_Tab>();
	CreateIconTabData(MouseTab, FName("MouseTab"));
	{

	}
}

void UYCNListData_RegisterKeyMouseData::CreateIconTabData(UYCNListData_Tab* InData, const FName& InDataID, UYCNListData_Base* InParentData)
{
	if (!IsValid(InData) || InDataID.IsNone() || (InParentData && !IsValid(InParentData))) return;
    
    InData->SetDataID(InDataID);

    if (InParentData)
    {
        InParentData->AddChildData(InData);
    }
    else
    {
        KeyboardMouseDataList.Add(InData);
    }
}
