// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_Options.h"
#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_DetailsBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ListViewBase.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Value.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "Components/Border.h"

#include "YCNUIDemo/DebugHelper/YCNDebugHelper.h"

void UYCNWidget_Options::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ResetAction, true, FSimpleDelegate::CreateUObject(this, &UYCNWidget_Options::OnClicked_Reset)));
		if (ResetActionHandle.IsValid())
		{
			//ResetActionHandle 有效就移除
			RemoveActionBinding(ResetActionHandle);
		}
		bResetActionAdded = false;
	}

	if (IsValid(TabList_Tab))
	{
		TabList_Tab->OnTabSelected.AddDynamic(this, &UYCNWidget_Options::OnTabSelected_Tab);
	}
	if (IsValid(ListView_SubTab))
	{
		ListView_SubTab->SetSelectionMode(ESelectionMode::Single);
		ListView_SubTab->OnEntryWidgetGenerated().AddUObject(this, &UYCNWidget_Options::OnListEntryGenerated);
		ListView_SubTab->OnItemSelectionChanged().AddUObject(this, &UYCNWidget_Options::OnSubTabSelectionChanged);
	}
	if (IsValid(Border_DropdownBlocker))
	{
		Border_DropdownBlocker->OnMouseButtonDownEvent.BindDynamic(this, &UYCNWidget_Options::OnDropdownBlockerMouseButtonDown);
	}

	//页面初始化时先隐藏遮罩
	SetDropdownBlockerVisible(false);
}

void UYCNWidget_Options::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UYCNWidget_Options::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (IsValid(InitOwningRegisterData()) && IsValid(ListView_SubTab) && IsValid(TabList_Tab))
	{
		if (OwningRegisterData)
		{
			FName FirstTabID = NAME_None;

			for (const UYCNListData_Tab* AvailableTab : OwningRegisterData->GetAvailableDataList())
			{
				if (!IsValid(AvailableTab)) continue;

				const FName TabID = AvailableTab->GetDataID();
				if (TabID.IsNone())continue;

				if (!TabList_Tab->GetTabButtonBaseByID(TabID))
				{
					//对应的 ID 没有对应的按钮就添加该按钮
					TabList_Tab->ApplyRegisterTab(AvailableTab);
				}

				if (FirstTabID.IsNone() && TabList_Tab->GetTabButtonBaseByID(TabID))
				{
					//将 FirstTabID 设置为第一次循环成功的 ID
					FirstTabID = TabID;
				}
			}
			//当前选中的主标签的ID
			FName SelectedTabID = TabList_Tab->GetSelectedTabId();

			if (SelectedTabID.IsNone() && !FirstTabID.IsNone())
			{
				//如果当前选中的主标签ID为空 就设置为第一个 标签 ID
				TabList_Tab->SelectTabByID(FirstTabID);
				SelectedTabID = TabList_Tab->GetSelectedTabId();
			}

			OnTabSelected(SelectedTabID);
		}
	}
}

void UYCNWidget_Options::NativeOnDeactivated()
{
	if (SelectedDropdownEntry.IsValid())
	{
		SelectedDropdownEntry->CloseDropdownMenu(false);
	}

	SelectedDropdownEntry.Reset();
	SetDropdownBlockerVisible(false);

	Super::NativeOnDeactivated();
}

void UYCNWidget_Options::OnClicked_Reset()
{
	YCNDebug::Print("重置");
}

void UYCNWidget_Options::OnTabSelected(FName TabID)
{
	if (!IsValid(ListView_SubTab))return;

	AddResetAction(TabID);

	UYCNListData_Tab* SelectedTab = nullptr;

	if (IsValid(OwningRegisterData) && !TabID.IsNone())
	{
		SelectedTab = OwningRegisterData->GetDataByDataID(TabID);
	}

	ListView_SubTab->ClearSelection();

	if (!IsValid(SelectedTab))
	{
		ListView_SubTab->ClearListItems();
		return;
	}

	ListView_SubTab->SetListItems(SelectedTab->GetChildDataList());

	if (IsValid(SelectedTab))
	{
		//更新详情面板
		UpdateDetails(SelectedTab);
	}
	
	if (!ListView_SubTab->GetListItems().IsEmpty())
	{
		ListView_SubTab->SetSelectedIndex(0);
		ListView_SubTab->NavigateToIndex(0);
	}
	ListView_SubTab->ScrollToTop();
}

void UYCNWidget_Options::OnTabSelected_Tab(FName TabId)
{
	OnTabSelected(TabId);
}

UYCNListData_RegisterData* UYCNWidget_Options::InitOwningRegisterData()
{
	if (!IsValid(OwningRegisterData))
	{
		OwningRegisterData = NewObject<UYCNListData_RegisterData>(this);
		OwningRegisterData->InitDataList();
		OwningRegisterData->OnSettingValueChanged.BindUObject(this, &UYCNWidget_Options::OnSettingValueChanged);
	}

	return OwningRegisterData.Get();
}

void UYCNWidget_Options::OnListEntryGenerated(UUserWidget& EntryWidget)
{
	UYCNWidget_Entry_Value* ValueEntry = Cast<UYCNWidget_Entry_Value>(&EntryWidget);
	if (IsValid(ValueEntry))
	{
		ValueEntry->OnDropdownOpenChanged.BindUObject(this, &UYCNWidget_Options::OnEntryDropdownOpenChanged, ValueEntry);
	}
}

void UYCNWidget_Options::SetDropdownBlockerVisible(bool bIsOpen)
{
	if (Border_DropdownBlocker)
	{
		Border_DropdownBlocker->SetVisibility(bIsOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UYCNWidget_Options::OnEntryDropdownOpenChanged(bool bIsOpen, UYCNWidget_Entry_Value* SourceEntry)
{
	if (!IsValid(Border_DropdownBlocker))return;

	if (bIsOpen)
	{
		if (SelectedDropdownEntry.IsValid() && SelectedDropdownEntry.Get() != SourceEntry)
		{
			SelectedDropdownEntry->CloseDropdownMenu(false);
		}

		SelectedDropdownEntry = SourceEntry;
		SetDropdownBlockerVisible(true);
	}
	else if (SelectedDropdownEntry.Get() == SourceEntry)
	{
		SelectedDropdownEntry.Reset();
		SetDropdownBlockerVisible(false);
	}
}

void UYCNWidget_Options::OnSubTabSelectionChanged(UObject* InSelectedItem)
{
	if (!IsValid(DetailsBase_Details))return;

	UniqueSelectedItem = Cast<UYCNListData_Base>(InSelectedItem);

	if (UniqueSelectedItem.IsValid())
	{
		DetailsBase_Details->SetDataDetails(UniqueSelectedItem.Get()->GetDataDisplayText(), UniqueSelectedItem.Get()->GetDataDescription());
	}
	else
	{
		DetailsBase_Details->SetDataDetails(FText::GetEmpty(), FText::GetEmpty());
	}
}

void UYCNWidget_Options::UpdateDetails(UYCNListData_Tab* InData)
{
	if (!DetailsBase_Details)return;
	
	if (InData->GetChildDataList().IsEmpty())
	{
		UE_LOG(LogYCNCommonUI, Error, TEXT("%s没有子数据!"), *InData->GetDataDisplayText().ToString());
		DetailsBase_Details->SetDataDetails(FText::GetEmpty(), FText::GetEmpty());
	}
	else
	{
		TArray<UYCNListData_Base*>ChildData = InData->GetChildDataList();

		const UYCNListData_Base* FirstData = ChildData.IsEmpty() ? nullptr : ChildData[0];

		if (IsValid(FirstData))
		{
			
			DetailsBase_Details->SetDataDetails(FirstData->GetDataDisplayText(), FirstData->GetDataDescription());
		}
	}
}

void UYCNWidget_Options::AddResetAction(const FName& InTabID)
{
	if (!ResetActionHandle.IsValid() || !IsValid(OwningRegisterData) || InTabID.IsNone())return;

	bool bShouldShowReset = false;

	UYCNListData_Tab* SelctedTab = OwningRegisterData->GetDataByDataID(InTabID);
	if (!SelctedTab)return;

	for (UYCNListData_Base* ChildData : SelctedTab->GetChildDataList())
	{
		UYCNListData_Value* ValueData = Cast<UYCNListData_Value>(ChildData);
		if (!ValueData)continue;

		FText CurrentText;
		if (!ValueData->TryReadSettingText(CurrentText))continue;

		const FText& DefaultValue = ValueData->GetOwningDefaultValue();
		if (CurrentText.IsEmpty())
		{
			CurrentText = DefaultValue;
		}

		if (!CurrentText.EqualTo(DefaultValue))
		{
			bShouldShowReset = true;
			break;
		}
	}

	//如果没有需要绑定的就返回
	if (bShouldShowReset == bResetActionAdded)return;

	if (bShouldShowReset)
	{
		AddActionBinding(ResetActionHandle);
	}
	else
	{
		RemoveActionBinding(ResetActionHandle);
	}

	bResetActionAdded = bShouldShowReset;
}

void UYCNWidget_Options::OnSettingValueChanged()
{
	const FName CurrentTabID = IsValid(TabList_Tab) ? TabList_Tab->GetSelectedTabId() : NAME_None;

	AddResetAction(CurrentTabID);
}

FEventReply UYCNWidget_Options::OnDropdownBlockerMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (SelectedDropdownEntry.IsValid())
	{
		SelectedDropdownEntry->CloseDropdownMenu();
	}

	// 同时处理条目已失效等情况，避免残留遮罩
	SelectedDropdownEntry.Reset();
	SetDropdownBlockerVisible(false);

	// 消费这次鼠标按下，不继续执行底层页面操作
	return FEventReply(true);
}
