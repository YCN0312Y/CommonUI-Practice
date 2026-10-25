// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_Options.h"
#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ListViewBase.h"
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

	if (IsValid(TabList_MainTab))
	{
		TabList_MainTab->OnTabSelected.AddDynamic(this, &UYCNWidget_Options::OnTabSelected_MainTab);
	}
}

void UYCNWidget_Options::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UYCNWidget_Options::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (IsValid(InitOwningRegisterData()) && IsValid(ListView_SubTab) && IsValid(TabList_MainTab))
	{
		if (OwningRegisterData)
		{
			FName FirstTabID = NAME_None;

			for (const UYCNListData_Tab* AvailableTab : OwningRegisterData->GetAvailableDataList())
			{
				if (!IsValid(AvailableTab)) continue;

				const FName TabID = AvailableTab->GetDataID();
				if (TabID.IsNone())continue;

				if (!TabList_MainTab->GetTabButtonBaseByID(TabID))
				{
					//对应的 ID 没有对应的按钮就添加该按钮
					TabList_MainTab->ApplyRegisterTab(AvailableTab);
				}

				if (FirstTabID.IsNone() && TabList_MainTab->GetTabButtonBaseByID(TabID))
				{
					//将 FirstTabID 设置为第一次循环成功的 ID
					FirstTabID = TabID;
				}
			}
			//当前选中的主标签的ID
			FName SelectedTabID = TabList_MainTab->GetSelectedTabId();

			if (SelectedTabID.IsNone() && !FirstTabID.IsNone())
			{
				//如果当前选中的主标签ID为空 就设置为第一个 标签 ID
				TabList_MainTab->SelectTabByID(FirstTabID);
				SelectedTabID = TabList_MainTab->GetSelectedTabId();
			}

			OnMainTabSelected(SelectedTabID);
		}
	}
}

void UYCNWidget_Options::OnMainTabSelected(FName TabID)
{
	if (!IsValid(ListView_SubTab))return;

	TWeakObjectPtr<UYCNListData_Tab>SelectedTab = nullptr;

	if (IsValid(OwningRegisterData) && !TabID.IsNone())
	{
		SelectedTab = OwningRegisterData->GetDataByDataID(TabID);
	}

	ListView_SubTab->ClearSelection();

	if (!SelectedTab.IsValid())
	{
		ListView_SubTab->ClearListItems();
		return;
	}

	ListView_SubTab->SetListItems(SelectedTab->GetChildDataList());
	ListView_SubTab->ScrollToTop();
}

void UYCNWidget_Options::OnTabSelected_MainTab(FName TabId)
{
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
