// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_Options.h"
#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"

#include "YCNUIDemo/DebugHelper/YCNDebugHelper.h"

void UYCNWidget_Options::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ResetAction, true, FSimpleDelegate::CreateUObject(this, &UYCNWidget_Options::OnClickedReset)));
	}

	if (!ICommonInputModule::GetSettings().GetDefaultBackAction().IsNull())
	{
		RegisterUIActionBinding(
			FBindUIActionArgs(
				ICommonInputModule::GetSettings().GetDefaultBackAction(), 
				true, FSimpleDelegate::CreateUObject(this, &UYCNWidget_Options::OnClickedBack)));
	}
}

void UYCNWidget_Options::NativePreConstruct()
{
	Super::NativePreConstruct();

	InitOwningRegisterData();

	if (IsValid(TabList_MainTab))
	{
		if (OwningRegisterData)
		{
			for (const UYCNListData_Tab* AvailableTab : OwningRegisterData->GetAvailableDataList())
			{
				if (!AvailableTab) continue;

				if (TabList_MainTab->GetTabButtonBaseByID(AvailableTab->GetDataID())) continue;

				TabList_MainTab->ApplyRegisterTab(AvailableTab->GetDataID());
			}
		}
	}
}

void UYCNWidget_Options::OnClickedBack()
{
	DeactivateWidget();
}

void UYCNWidget_Options::OnClickedReset()
{
	YCNDebug::Print("重置");
}

UYCNListData_RegisterData* UYCNWidget_Options::InitOwningRegisterData()
{
	if (!IsValid(OwningRegisterData))
	{
		OwningRegisterData = NewObject<UYCNListData_RegisterData>(this);
		OwningRegisterData->InitDataList();
	}

	return OwningRegisterData.Get();
}
