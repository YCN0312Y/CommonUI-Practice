// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_KeyboardMouse.h"
#include "YCNUIDemo/UI/ListData/YCNListData_RegisterKeyMouseData.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"
#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ListViewBase.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Image.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/DebugHelper/YCNDebugHelper.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "Input/Events.h"
#include "Widgets/SWidget.h"
#include "Input/CommonUIInputTypes.h"

void UYCNWidget_KeyboardMouse::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (IsValid(TabList_Key))
	{
		TabList_Key->OnTabSelected.AddDynamic(this, &UYCNWidget_KeyboardMouse::OnTabSelected);
	}
	if (IsValid(ListView_Key))
	{
		ListView_Key->OnItemClicked().AddUObject(this, &UYCNWidget_KeyboardMouse::OnKeyItemClicked);
	}
	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ResetAction, true, FSimpleDelegate::CreateUObject(this, &UYCNWidget_KeyboardMouse::OnClicked_Reset)));
		if (ResetActionHandle.IsValid())
		{
			RemoveActionBinding(ResetActionHandle);
		}
	}
}

void UYCNWidget_KeyboardMouse::NativePreConstruct()
{
	Super::NativePreConstruct();
	StopKeyInputProcessor();
}

void UYCNWidget_KeyboardMouse::NativeOnActivated()
{
	Super::NativeOnActivated();

	//初始化标签列表
	InitTabList();
}

void UYCNWidget_KeyboardMouse::NativeOnDeactivated()
{
	StopKeyInputProcessor();

	Super::NativeOnDeactivated();
}

void UYCNWidget_KeyboardMouse::NativeDestruct()
{
	StopKeyInputProcessor();

	Super::NativeDestruct();
}

void UYCNWidget_KeyboardMouse::InitTabList()
{
	if (!IsValid(InitOwningRegisterData()) || !IsValid(ListView_Key) || !IsValid(TabList_Key))return;

	FName FirstTabID = NAME_None;

	for (const UYCNListData_Tab* AvailableTab : OwningRegisterData->GetKeyboardMouseDataList())
	{
		if (!IsValid(AvailableTab)) return;

		const FName TabID = AvailableTab->GetDataID();

		if (TabID.IsNone()) return;

		if (!TabList_Key->GetTabButtonBaseByID(TabID))
		{
			TabList_Key->ApplyRegisterTab(AvailableTab);
		}

		if (FirstTabID.IsNone() && TabList_Key->GetTabButtonBaseByID(TabID))
		{
			FirstTabID = TabID;
		}
	}

	FName SelectedTabID = TabList_Key->GetSelectedTabId();

	if (SelectedTabID.IsNone() && !FirstTabID.IsNone())
	{
		TabList_Key->SelectTabByID(FirstTabID);
		SelectedTabID = TabList_Key->GetSelectedTabId();
	}

	OnTabSelected(SelectedTabID);
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
	StopKeyInputProcessor();

	if (!IsValid(OwningRegisterData) || !IsValid(ListView_Key)) return;
	UYCNListData_Tab* FoundDataTab = OwningRegisterData->GetDataByDataID(InTabID);
	AddResetAction(InTabID);
	ListView_Key->ClearSelection();

	if (!IsValid(FoundDataTab))
	{
		ListView_Key->ClearListItems();
		return;
	}

	const TArray<UYCNListData_Base*> ChildDataList = FoundDataTab->GetChildDataList();

	ListView_Key->SetListItems(ChildDataList);

	if (!ChildDataList.IsEmpty())
	{
		ListView_Key->SetSelectedItem(ChildDataList[0]);
	}

}

void UYCNWidget_KeyboardMouse::OnKeyItemClicked(UObject* InItem)
{
	if (!IsValid(ListView_Key) || !FSlateApplication::IsInitialized()) return;

	UYCNListData_Image* ClickedData = Cast<UYCNListData_Image>(InItem);
	if (!IsValid(ClickedData))return;

	//已经在捕获时
	if (KeyInputProcessor.IsValid()) return;

	//保存当前正在等待输入的数据
	PendingIconData = ClickedData;
	KeyInputProcessor = MakeShared<FYCNKeyInputProcessor>();
	KeyInputProcessor->OnClickDelegate.BindUObject(this, &UYCNWidget_KeyboardMouse::OnClick_Key);

	const bool bRegistered = FSlateApplication::Get().RegisterInputPreProcessor(KeyInputProcessor, 0);

	if (!bRegistered)
	{
		StopKeyInputProcessor();
		return;
	}

	//确认注册成功后显示等待输入的图片
	SetItemWaitingForInput(PendingIconData.Get(), true);
}

void UYCNWidget_KeyboardMouse::SetItemWaitingForInput(UYCNListData_Image* InData, bool bWaiting)
{
	if (!IsValid(InData) || !IsValid(ListView_Key))return;

	InData->SetWaitingForInput(bWaiting);

	UYCNWidget_Entry_Image* EntryWidget = ListView_Key->GetEntryWidgetFromItem<UYCNWidget_Entry_Image>(InData);
	if (IsValid(EntryWidget))
	{
		EntryWidget->RefreshEntryData();
	}
}

void UYCNWidget_KeyboardMouse::StopKeyInputProcessor()
{
	if (!KeyInputProcessor.IsValid()) return;

	SetItemWaitingForInput(PendingIconData.Get(), false);
	PendingIconData = nullptr;

	//取消绑定
	KeyInputProcessor->OnClickDelegate.Unbind();

	if (FSlateApplication::IsInitialized())
	{
		//取消注册
		FSlateApplication::Get().UnregisterInputPreProcessor(KeyInputProcessor);
	}
	//重置
	KeyInputProcessor.Reset();
}

void UYCNWidget_KeyboardMouse::OnClick_Key(const FKey& InKey)
{
	if (!InKey.IsValid() || !IsValid(TabList_Key) || !IsValid(PendingIconData)) return;

	UYCNListData_Image* TargetData = PendingIconData.Get();

	if (!IsValid(TargetData) || InKey == EKeys::Escape)
	{
		//如果是Esc键就返回
		StopKeyInputProcessor();
		return;
	}
	bool bUsedByOtherAction = false;

	if (IsValid(OwningRegisterData))
	{
		// 同时检查键盘和鼠标标签中的动作。
		for (UYCNListData_Tab* TabData : OwningRegisterData->GetKeyboardMouseDataList())
		{
			if (!IsValid(TabData))continue;
			
			for (UYCNListData_Base* ChildData : TabData->GetChildDataList())
			{
				const UYCNListData_Image* OtherData = Cast<UYCNListData_Image>(ChildData);

				//排除当前正在修改的条目
				if (!IsValid(OtherData) || OtherData == TargetData) continue;
				
				if (OtherData->GetPreviewKey() == InKey)
				{
					bUsedByOtherAction = true;
					break;
				}
			}

			if (bUsedByOtherAction) break;
		}
	}

	if (bUsedByOtherAction)
	{
		if (IsValid(ListView_Key))
		{
			UYCNWidget_Entry_Image* EntryWidget = ListView_Key->GetEntryWidgetFromItem<UYCNWidget_Entry_Image>(TargetData);

			if (IsValid(EntryWidget))
			{
				EntryWidget->SetClickTextStyleAndText();
			}
		}

		return;
	}

	FSlateBrush KeyBrush;

	if (UYCNFunctionLibrary::TryGetKeyBrush(InKey, KeyBrush))
	{
		if (TargetData->GetSettingBinding().TryWrite(InKey))
		{
			CurrentKey = InKey;
		}
	}
	else
	{
		UE_LOG(LogYCNCommonUI, Error, TEXT("没有配置这个按键的图标：%s"), *InKey.ToString());
	}

	StopKeyInputProcessor();

	AddResetAction(TabList_Key->GetSelectedTabId());
}

void UYCNWidget_KeyboardMouse::OnClicked_Reset()
{
	if (!IsValid(OwningRegisterData) || !IsValid(TabList_Key)) return;

	//当前选择的标签ID
	const FName CurrentSelectedTabID = TabList_Key->GetSelectedTabId();
	//当前标签
	UYCNListData_Tab* CurrentTab = OwningRegisterData->GetDataByDataID(CurrentSelectedTabID);

	UYCNWidgetManagerSubsystem* Manager = UYCNWidgetManagerSubsystem::Get(this);

	if (!IsValid(CurrentTab) || !IsValid(Manager)) return;

	//记录点击重置时的页面，回调执行时检查对象是否仍有效。
	TWeakObjectPtr<UYCNListData_Tab> WeakTab = CurrentTab;

	Manager->PushConfirmationToStack(
		EConfirmationWindowsType::YesNO,
		FText::FromString(TEXT("确定重置当前页面所有内容吗？")),
		[WeakTab, this](EConfirmationWindowsButtonType ButtonType)
		{
			if (ButtonType != EConfirmationWindowsButtonType::Yes) return;

			//要重置的标签页
			UYCNListData_Tab* TabToReset = WeakTab.Get();
			if (!IsValid(TabToReset))return;
			for (UYCNListData_Base* ChildData : TabToReset->GetChildDataList())
			{
				UYCNListData_Image* ImageData = Cast<UYCNListData_Image>(ChildData);

				if (!IsValid(ImageData)) continue;
			
				ImageData->ResetToDefault();
			}

			if (IsValid(ListView_Key))
			{
				for (UUserWidget* EntryWidget : ListView_Key->GetDisplayedEntryWidgets())
				{
					UYCNWidget_Entry_Image* ImageEntry = Cast<UYCNWidget_Entry_Image>(EntryWidget);

					if (IsValid(ImageEntry))
					{
						//刷新条目数据
						ImageEntry->RefreshEntryData();
					}
				}
			}

			//隐藏重置按钮
			if (IsValid(TabList_Key))
			{
				AddResetAction(TabList_Key->GetSelectedTabId());
			}
		});
}

void UYCNWidget_KeyboardMouse::AddResetAction(const FName& InTabID)
{
	if (!ResetActionHandle.IsValid()) return;
	
	bool bShouldShowReset = false;

	if (IsValid(OwningRegisterData) && !InTabID.IsNone())
	{
		UYCNListData_Tab* CurrentTab = OwningRegisterData->GetDataByDataID(InTabID);

		if (IsValid(CurrentTab))
		{
			// 当前键盘、鼠标条目都直接放在标签下面。
			for (UYCNListData_Base* ChildData : CurrentTab->GetChildDataList())
			{
				const UYCNListData_Image* ImageData = Cast<UYCNListData_Image>(ChildData);

				if (IsValid(ImageData) && ImageData->CanResetToDefault())
				{
					bShouldShowReset = true;
					break;
				}
			}
		}
	}

	if (bShouldShowReset == bResetActionAdded) return;
	
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

//===================================================================
//FYCNKeyInputProcessor按键输入处理器
//===================================================================
void FYCNKeyInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{

}

bool FYCNKeyInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if (!InKeyEvent.GetKey().IsValid()) return false;

	//保证外部回调清理处理器时，本次函数执行期间对象仍然存活
	const TSharedRef<FYCNKeyInputProcessor> KeepAlive = AsShared();

	//使用委托副本，允许回调解除原委托的绑定
	const FOnClickDelegate Callback = OnClickDelegate;
	Callback.ExecuteIfBound(InKeyEvent.GetKey());

	return true;
}

bool FYCNKeyInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (!MouseEvent.GetEffectingButton().IsValid())return false;

	const TSharedRef<FYCNKeyInputProcessor> KeepAlive = AsShared();

	const FOnClickDelegate Callback = OnClickDelegate;
	Callback.ExecuteIfBound(MouseEvent.GetEffectingButton());

	return true;
}