// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_KeyboardMouse.h"
#include "YCNUIDemo/UI/ListData/YCNListData_RegisterKeyMouseData.h"
#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ListViewBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"

void UYCNWidget_KeyboardMouse::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UYCNWidget_KeyboardMouse::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UYCNWidget_KeyboardMouse::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (IsValid(InitOwningRegisterData()) && IsValid(ListView_Key) && IsValid(TabList_Key))
	{
		if (IsValid(OwningRegisterData.Get()))
		{
			FName FirstTabID = NAME_None;

			for (const UYCNListData_Tab* AvailableTab : OwningRegisterData->GetKeyboardMouseDataList())
			{
				if (!IsValid(AvailableTab)) continue;

				const FName TabID = AvailableTab->GetDataID();
				if (TabID.IsNone())continue;

				if (!TabList_Key->GetTabButtonBaseByID(TabID))
				{
					//对应的 ID 没有对应的按钮就添加该按钮
					TabList_Key->ApplyRegisterTab(AvailableTab);
				}

				if (FirstTabID.IsNone() && TabList_Key->GetTabButtonBaseByID(TabID))
				{
					//将 FirstTabID 设置为第一次循环成功的 ID
					FirstTabID = TabID;
				}
			}
			//当前选中的主标签的ID
			FName SelectedTabID = TabList_Key->GetSelectedTabId();

			if (SelectedTabID.IsNone() && !FirstTabID.IsNone())
			{
				//如果当前选中的主标签ID为空 就设置为第一个 标签 ID
				TabList_Key->SelectTabByID(FirstTabID);
				SelectedTabID = TabList_Key->GetSelectedTabId();
			}

			OnTabSelected(SelectedTabID);
		}
	}
}

void UYCNWidget_KeyboardMouse::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

}

UYCNListData_RegisterKeyMouseData* UYCNWidget_KeyboardMouse::InitOwningRegisterData()
{
	if (!IsValid(OwningRegisterData))
	{
		OwningRegisterData = NewObject<UYCNListData_RegisterKeyMouseData>();
		OwningRegisterData->InitDataList();
	}
	return OwningRegisterData;
}

void UYCNWidget_KeyboardMouse::OnTabSelected(FName InTabID)
{

}