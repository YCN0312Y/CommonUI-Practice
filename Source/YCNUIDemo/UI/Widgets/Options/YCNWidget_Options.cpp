// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_Options.h"
#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_DetailsBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_RegisterData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/UI/ListData/YCNListData_String.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ListViewBase.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_StringOption.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "Components/Border.h"
#include "YCNUIDemo/Msic/YCNDataType.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_MainStack.h"
#include "YCNUIDemo/Tags/YCNGameplayTags.h"

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
		TabList_Tab->OnTabSelected.AddDynamic(this, &UYCNWidget_Options::OnTabSelected);
	}
	if (IsValid(ListView_SubTab))
	{
		ListView_SubTab->SetSelectionMode(ESelectionMode::Single);
		ListView_SubTab->OnEntryWidgetGenerated().AddUObject(this, &UYCNWidget_Options::OnListEntryGenerated);
		ListView_SubTab->OnItemSelectionChanged().AddUObject(this, &UYCNWidget_Options::OnSubTabSelectionChanged);
	}
	if (IsValid(ListView_Control))
	{
		ListView_Control->SetSelectionMode(ESelectionMode::Single);
		ListView_Control->OnEntryWidgetGenerated().AddUObject(this, &UYCNWidget_Options::OnListEntryGenerated);
		ListView_Control->OnItemSelectionChanged().AddUObject(this, &UYCNWidget_Options::OnSubTabSelectionChanged);
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

bool UYCNWidget_Options::NativeOnHandleBackAction()
{
	if (bLeaveConfirmationOpen) return true;

	UYCNGameUserSettings* Settings = UYCNGameUserSettings::Get();

	if (!Settings || !Settings->HasPendingQualityChanges())
	{
		// 真正退出选项页面前，恢复返回主菜单的过渡时间。
		SetStackTransitionDuration(YCNGameplayTags::YCN_WidgetStack_Menu, 0.4f);

		return Super::NativeOnHandleBackAction();
	}

	ConfirmQualityBeforeLeaving(NAME_None);
	return true;
}

void UYCNWidget_Options::OnClicked_Reset()
{
	if (!IsValid(OwningRegisterData) || !IsValid(TabList_Tab)) return;
	
	//当前选择的标签ID
	const FName CurrentSelectedTabID = TabList_Tab->GetSelectedTabId();
	//当前标签
	UYCNListData_Tab* CurrentTab = OwningRegisterData->GetDataByDataID(CurrentSelectedTabID);

	UYCNWidgetManagerSubsystem* Manager = UYCNWidgetManagerSubsystem::Get(this);

	if (!IsValid(CurrentTab) || !IsValid(Manager)) return;
	
	//记录点击重置时的页面，回调执行时检查对象是否仍有效。
	TWeakObjectPtr<UYCNListData_Tab> WeakTab = CurrentTab;

	Manager->PushConfirmationToStack(
		EConfirmationWindowsType::YesNO,
		FText::FromString(TEXT("确定重置当前页面所有内容吗？")),
		[WeakTab](EConfirmationWindowsButtonType ButtonType)
		{
			// 点击取消时不执行重置。
			if (ButtonType != EConfirmationWindowsButtonType::Yes) return;

			//要重置的标签页
			UYCNListData_Tab* TabToReset = WeakTab.Get();
			if (!IsValid(TabToReset)) return;
			
			TFunction<void(UYCNListData_Base*)> ResetNode;

			ResetNode = [&ResetNode](UYCNListData_Base* Data)
				{
					if (!IsValid(Data)) return;
					
					if (UYCNListData_Value* ValueData = Cast<UYCNListData_Value>(Data))
					{
						ValueData->ResetToDefault();
					}

					for (UYCNListData_Base* ChildData : Data->GetChildDataList())
					{
						ResetNode(ChildData);
					}
				};
			ResetNode(TabToReset);
		});
}

void UYCNWidget_Options::OnTabSelected(FName TabID)
{
	if (bRestoringTab || TabID.IsNone()) return;

	if (DisplayedTabID.IsNone())
	{
		ShowTabContent(TabID);
		return;
	}

	if (TabID == DisplayedTabID) return;

	UYCNGameUserSettings* Settings = UYCNGameUserSettings::Get();
	if (!bLeaveConfirmationOpen && (!Settings || !Settings->HasPendingQualityChanges()))
	{
		ShowTabContent(TabID);
		return;
	}

	TGuardValue<bool> Guard(bRestoringTab, true);
	TabList_Tab->SelectTabByID(DisplayedTabID, true);

	if (!bLeaveConfirmationOpen)
	{
		ConfirmQualityBeforeLeaving(TabID);
	}
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
	UYCNWidget_Entry_StringOption* ValueEntry = Cast<UYCNWidget_Entry_StringOption>(&EntryWidget);
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

void UYCNWidget_Options::OnEntryDropdownOpenChanged(bool bIsOpen, UYCNWidget_Entry_StringOption* SourceEntry)
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
	if (!ResetActionHandle.IsValid())return;

	bool bShouldShowReset = false;

	if (IsValid(OwningRegisterData) && !InTabID.IsNone())
	{
		UYCNListData_Tab* SelectedTab = OwningRegisterData->GetDataByDataID(InTabID);

		if (IsValid(SelectedTab))
		{
			// 包含分组下面的所有子数据。
			const TArray<UYCNListData_Base*> AllChildData = GetTabAllChildData(SelectedTab, InTabID);

			for (UYCNListData_Base* ChildData : AllChildData)
			{
				const UYCNListData_Value* ValueData = Cast<UYCNListData_Value>(ChildData);

				if (IsValid(ValueData) && ValueData->CanResetToDefault())
				{
					bShouldShowReset = true;
					break;
				}
			}
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
	AddResetAction(DisplayedTabID);

	const bool bIsControlTab = DisplayedTabID == FName(TEXT("ControllerTab")) || DisplayedTabID == FName(TEXT("KeyboardMouseTab"));

	UYCNWidget_ListViewBase* CurrentList = bIsControlTab ? ListView_Control.Get() : ListView_SubTab.Get();

	if (!IsValid(CurrentList)) return;
	
	for (UUserWidget* EntryWidget : CurrentList->GetDisplayedEntryWidgets())
	{
		if (UYCNWidget_EntryBase* Entry = Cast<UYCNWidget_EntryBase>(EntryWidget))
		{
			Entry->RefreshEntryData();
		}
	}
}

const TArray<UYCNListData_Base*> UYCNWidget_Options::GetTabAllChildData(UYCNListData_Tab* InTab, const FName& InTabID)
{
	TArray<UYCNListData_Base*> Result;

	if (!IsValid(OwningRegisterData) || InTabID.IsNone()) return Result;

	//当前选择的标签
	InTab = OwningRegisterData->GetDataByDataID(InTabID);

	if (!IsValid(InTab)) return Result;
	
	//收集一个节点的所有子节点。
	TFunction<void(UYCNListData_Base*)> CollectChildren;

	CollectChildren =
		[&Result, &CollectChildren](UYCNListData_Base* Parent)
		{
			if (!IsValid(Parent)) return;
			
			for (UYCNListData_Base* Child : Parent->GetChildDataList())
			{
				if (!IsValid(Child)) continue;
				
				Result.Add(Child);

				CollectChildren(Child);
			}
		};

	CollectChildren(InTab);

	return Result;
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

void UYCNWidget_Options::ShowTabContent(FName TabID)
{
	if (!IsValid(ListView_Control) || !IsValid(ListView_SubTab))
	{
		return;
	}

	const bool bIsControlTab = TabID == FName(TEXT("ControllerTab")) || TabID == FName(TEXT("KeyboardMouseTab"));

	// 切换分类前，关闭上一页可能打开的下拉菜单。
	if (SelectedDropdownEntry.IsValid())
	{
		SelectedDropdownEntry->CloseDropdownMenu(false);
	}
	if (bIsControlTab)
	{
		//是控制器或者按键标签就将堆栈过渡时间设置为0
		SetStackTransitionDuration(YCNGameplayTags::YCN_WidgetStack_Menu, 0.f);
	}
	else
	{
		SetStackTransitionDuration(YCNGameplayTags::YCN_WidgetStack_Menu, 0.4f);
	}

	SelectedDropdownEntry.Reset();
	SetDropdownBlockerVisible(false);

	// 清除旧页面的选择状态。
	ListView_Control->ClearSelection();
	ListView_SubTab->ClearSelection();
	UniqueSelectedItem.Reset();

	ListView_Control->SetVisibility(bIsControlTab ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	ListView_SubTab->SetVisibility(bIsControlTab ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	// 控制器和键鼠页使用较宽的内容布局，不显示右侧详情。
	if (IsValid(DetailsBase_Details))
	{
		DetailsBase_Details->SetVisibility(bIsControlTab ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}

	DisplayedTabID = TabID;
	AddResetAction(TabID);

	UYCNWidget_ListViewBase* TargetList = bIsControlTab ? ListView_Control.Get() : ListView_SubTab.Get();

	AddDataToList(TabID, TargetList);
}

void UYCNWidget_Options::ConfirmQualityBeforeLeaving(FName TargetTabID)
{
	if (bLeaveConfirmationOpen) return;

	UYCNWidgetManagerSubsystem* Manager = UYCNWidgetManagerSubsystem::Get(this);
	if (!Manager) return;

	bLeaveConfirmationOpen = true;

	Manager->PushConfirmationToStack(
		EConfirmationWindowsType::YesNO,
		FText::FromString(TEXT("画质设置尚未应用，是否应用并继续？")),
		[this, TargetTabID]
		(EConfirmationWindowsButtonType ButtonType)
		{
			bLeaveConfirmationOpen = false;

			if (ButtonType != EConfirmationWindowsButtonType::Yes) return;

			UYCNGameUserSettings* Settings = UYCNGameUserSettings::Get();
			if (!Settings) return;

			if (Settings->HasPendingQualityChanges())
			{
				//没有待处理的画质就返回
				if (!Settings->ApplyPendingQuality()) return;
				OnSettingValueChanged();
			}

			if (TargetTabID.IsNone())
			{
				NativeOnHandleBackAction();
			}

			if (IsValid(TabList_Tab))
			{
				TabList_Tab->SelectTabByID(TargetTabID, true);
			}
		});
}

void UYCNWidget_Options::AddDataToList(FName TabID, UYCNWidget_ListViewBase* InList)
{
	if (!IsValid(InList))
	{
		return;
	}

	UYCNListData_Tab* SelectedTab = nullptr;

	if (IsValid(OwningRegisterData) && !TabID.IsNone())
	{
		SelectedTab = OwningRegisterData->GetDataByDataID(TabID);
	}

	InList->ClearSelection();

	if (!IsValid(SelectedTab))
	{
		InList->ClearListItems();

		if (IsValid(DetailsBase_Details))
		{
			DetailsBase_Details->SetDataDetails(FText::GetEmpty(), FText::GetEmpty());
		}
		return;
	}

	// 根据传入的列表，填充当前分类的数据。
	InList->SetListItems(GetTabAllChildData(SelectedTab, TabID));

	// 普通列表才需要更新右侧详情。
	if (InList == ListView_SubTab.Get())
	{
		UpdateDetails(SelectedTab);
	}

	if (!InList->GetListItems().IsEmpty())
	{
		InList->SetSelectedIndex(0);
		InList->NavigateToIndex(0);
	}

	InList->ScrollToTop();
}

void UYCNWidget_Options::SetStackTransitionDuration(const FGameplayTag& InStackTag, float InDuration)
{
	if (UYCNWidgetManagerSubsystem* Manager = UYCNWidgetManagerSubsystem::Get(this))
	{
		UYCNWidget_MainStack* MainStack = Manager->GetOwningMainStack();
		if (IsValid(MainStack))
		{
			Manager->GetOwningMainStack()->SetStackTransitionDuration(MainStack->FindWidgetByTag(InStackTag), InDuration);
		}
	}
}
