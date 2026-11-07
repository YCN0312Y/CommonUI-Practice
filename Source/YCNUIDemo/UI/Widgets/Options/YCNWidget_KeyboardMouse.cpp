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
#include "Engine/LocalPlayer.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "Input/Events.h"
#include "Widgets/SWidget.h"

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
}

void UYCNWidget_KeyboardMouse::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UYCNWidget_KeyboardMouse::NativeOnActivated()
{
	Super::NativeOnActivated();

	StopKeyInputProcessor();

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
	// 切换标签时，取消上一行的捕获。
	StopKeyInputProcessor();

	if (!IsValid(OwningRegisterData) || !IsValid(ListView_Key)) return;
	UYCNListData_Tab* FoundDataTab = OwningRegisterData->GetDataByDataID(InTabID);

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
	UYCNListData_Image* ClickedData = Cast<UYCNListData_Image>(InItem);

	if (!IsValid(ClickedData) || !ClickedData->HasPreviewKey())return;

	// 已在捕获中，避免重复启动。
	if (KeyInputProcessor.IsValid())return;

	if (!IsActivated() || !GetOwningPlayer() || !GetOwningLocalPlayer() || !FSlateApplication::IsInitialized())return;

	PendingIconData = ClickedData;

	// 将焦点交给当前页面，便于限定捕获范围。
	SetIsFocusable(true);
	SetUserFocus(GetOwningPlayer());

	FSlateApplication& SlateApp = FSlateApplication::Get();

	const int32 UserIndex = SlateApp.GetUserIndexForController(GetOwningLocalPlayer()->GetControllerId());

	KeyInputProcessor = MakeShared<FYCNKeyInputProcessor>(UserIndex, TakeWidget());

	KeyInputProcessor->OnKeyCaptured.BindUObject(this, &UYCNWidget_KeyboardMouse::OnCapturedKey);
	KeyInputProcessor->OnCaptureFinished.BindUObject(this, &UYCNWidget_KeyboardMouse::OnCaptureFinished);

	const bool bRegistered = SlateApp.RegisterInputPreProcessor(KeyInputProcessor, 0);

	if (!bRegistered)
	{
		StopKeyInputProcessor();
		return;
	}

	YCNDebug::Print(FString::Printf(TEXT("当前条目：%s，请按下按键，Esc 取消"), *ClickedData->GetDataDisplayText().ToString()));
}

void UYCNWidget_KeyboardMouse::StopKeyInputProcessor()
{
	PendingIconData = nullptr;

	if (!KeyInputProcessor.IsValid()) return;

	KeyInputProcessor->OnKeyCaptured.Unbind();
	KeyInputProcessor->OnCaptureFinished.Unbind();

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(KeyInputProcessor);
	}

	KeyInputProcessor.Reset();
}

void UYCNWidget_KeyboardMouse::OnCapturedKey(FKey PressedKey)
{
	UYCNListData_Image* TargetData = PendingIconData.Get();

	if (!IsValid(TargetData))return;

	FSlateBrush KeyBrush;

	// 先验证图标，缺图时不修改原来的 PreviewKey。
	if (!UYCNFunctionLibrary::TryGetKeyBrush(PressedKey, KeyBrush))
	{
		YCNDebug::Print(FString::Printf(TEXT("没有配置这个按键的图标：%s"), *PressedKey.ToString()));

		PendingIconData = nullptr;
		return;
	}

	// 更新条目数据，保证滚动复用后仍能显示新图。
	TargetData->SetPreviewKey(PressedKey);

	if (IsValid(ListView_Key))
	{
		UYCNWidget_Entry_Image* Entry = ListView_Key->GetEntryWidgetFromItem<UYCNWidget_Entry_Image>(TargetData);

		if (IsValid(Entry))
		{
			Entry->RefreshEntryData();
		}
	}

	YCNDebug::Print(FString::Printf(TEXT("%s 的图标已更换为：%s"), *TargetData->GetDataDisplayText().ToString(), *PressedKey.ToString()));

	// 图标更新完毕。
	// 处理器继续收尾，等按键松开后再注销。
	PendingIconData = nullptr;
}

void UYCNWidget_KeyboardMouse::OnCaptureFinished()
{
	StopKeyInputProcessor();

	if (IsActivated() && IsValid(ListView_Key) && GetOwningPlayer())
	{
		ListView_Key->SetUserFocus(GetOwningPlayer());
	}
}

//===================================================================
//FYCNKeyInputProcessor按键输入处理器
//===================================================================

FYCNKeyInputProcessor::FYCNKeyInputProcessor(int32 InUserIndex, TSharedRef<SWidget> InCaptureWidget)
	: UserIndex(InUserIndex),
	CaptureWidget(InCaptureWidget)
{

}

void FYCNKeyInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
	// 回调可能让页面注销处理器。
	// 在本次 Tick 返回前保持对象存活。
	const TSharedRef<FYCNKeyInputProcessor> KeepAlive = AsShared();

	if (bFinished)return;

	// 页面失去焦点或应用失活时，结束捕获。
	if (bCancelRequested || !IsInScope(SlateApp))
	{
		Finish();
		return;
	}

	if (!CapturedKey.IsValid()) return;


	// 下一次 Tick 派发按下结果，避免在按键事件中处理释放。
	if (!bResultDelivered)
	{
		bResultDelivered = true;

		// Escape 只取消，不改变图片。
		if (CapturedKey != EKeys::Escape)
		{
			const FOnKeyCaptured Callback = OnKeyCaptured;
			Callback.ExecuteIfBound(CapturedKey);
		}
	}

	// 图标可以先更新；保持处理器直到按键松开，
	// 避免松开事件继续触发下层 UI。
	if (PressedKeys.IsEmpty())
	{
		Finish();
	}
}

bool FYCNKeyInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetUserIndex() != UserIndex || !IsInScope(SlateApp) || bFinished)return false;

	// 等待期间消费重复输入，但不重复提交结果。
	if (InKeyEvent.IsRepeat())return true;

	PressKey(InKeyEvent.GetKey());
	return true;
}

bool FYCNKeyInputProcessor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& Event)
{
	if (Event.GetUserIndex() != UserIndex || !IsInScope(SlateApp) || bFinished)return false;

	PressedKeys.Remove(Event.GetKey());
	return true;
}

bool FYCNKeyInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetUserIndex() != UserIndex || !IsInScope(SlateApp) || bFinished) return false;

	// 点击捕获页面以外的区域，取消并放行该点击。
	if (!IsPointerInside(MouseEvent))
	{
		bCancelRequested = true;
		return false;
	}

	PressKey(MouseEvent.GetEffectingButton());
	return true;
}

bool FYCNKeyInputProcessor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& Event)
{
	if (Event.GetUserIndex() != UserIndex || !IsInScope(SlateApp) || bFinished)return false;

	PressedKeys.Remove(Event.GetEffectingButton());
	return true;
}

bool FYCNKeyInputProcessor::IsInScope(FSlateApplication& SlateApp) const
{
	const TSharedPtr<SWidget> Widget = CaptureWidget.Pin();

	if (!SlateApp.IsActive() || !Widget.IsValid())return false;

	return Widget->HasUserFocus(UserIndex).IsSet() || Widget->HasUserFocusedDescendants(UserIndex);
}

bool FYCNKeyInputProcessor::IsPointerInside(const FPointerEvent& Event) const
{
	const TSharedPtr<SWidget> Widget = CaptureWidget.Pin();

	return Widget.IsValid() && Widget->GetCachedGeometry().IsUnderLocation(Event.GetScreenSpacePosition());
}

void FYCNKeyInputProcessor::PressKey(const FKey& Key)
{
	if (!Key.IsValid())return;

	PressedKeys.Add(Key);
	// 一个会话只接受第一个有效键鼠按键。
	if (CapturedKey.IsValid() || Key.IsGamepadKey() || !Key.IsDigital())return;

	CapturedKey = Key;
}

void FYCNKeyInputProcessor::Finish()
{
	if (bFinished)return;

	bFinished = true;

	// 复制委托，允许接收方在回调里解除绑定。
	const FOnCaptureFinished Callback = OnCaptureFinished;

	Callback.ExecuteIfBound();
}