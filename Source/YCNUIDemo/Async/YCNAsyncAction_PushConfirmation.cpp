// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Async/YCNAsyncAction_PushConfirmation.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"

#include "YCNUIDemo/DebugHelper/YCNDebugHelper.h"

void UYCNAsyncAction_PushConfirmation::Activate()
{
	Super::Activate();

	UYCNWidgetManagerSubsystem* WidgetManager = UYCNWidgetManagerSubsystem::Get(CacedWorldContextObject);
	if (WidgetManager)
	{
		WidgetManager->PushConfirmationToStack(
			CachedWindowsType, CachedWindowMessageText,
			[this](EConfirmationWindowsButtonType ClickedButtonType)
			{
				OnButtonClicked.Broadcast(ClickedButtonType);
			});
	}
	SetReadyToDestroy();
}

UYCNAsyncAction_PushConfirmation* UYCNAsyncAction_PushConfirmation::PushConfrimationWindow(UObject* InWorldContextObject, EConfirmationWindowsType InWindowsType, FText InMessageText, TSoftClassPtr<UYCNWidget_ActivatableWidgetBase> InWidget)
{
	if (!IsValid(InWorldContextObject) || InWidget.IsNull())
	{
		UE_LOG(LogYCNCommonUI, Warning, TEXT("InWorldContextObject 或者 InWidget 无效！"));
		return nullptr;
	}
	UYCNAsyncAction_PushConfirmation* Node = NewObject<UYCNAsyncAction_PushConfirmation>();
	if (Node)
	{
		Node->CacedWorldContextObject = InWorldContextObject;
		Node->CachedWindowsType = InWindowsType;
		Node->CachedWindowMessageText = InMessageText;
		Node->CachedWidget = InWidget;

		return Node;
	}
	return nullptr;
}
