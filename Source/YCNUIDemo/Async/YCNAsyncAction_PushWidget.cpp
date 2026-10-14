// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Async/YCNAsyncAction_PushWidget.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"

#include "YCNUIDemo/DebugHelper/YCNDebugHelper.h"

void UYCNAsyncAction_PushWidget::Activate()
{
	Super::Activate();

	UYCNWidgetManagerSubsystem* WidgetManager = UYCNWidgetManagerSubsystem::Get(CacedWorldContextObject);

	if (WidgetManager)
	{
		WidgetManager->PushSoftWidgetToStack(CachedWidgetTag, CachedWidget);
	}

	SetReadyToDestroy();
}

UYCNAsyncAction_PushWidget* UYCNAsyncAction_PushWidget::PushSoftWidget(UObject* InWorldContextObject, FGameplayTag InWidgetTag, TSoftClassPtr<UYCNWidget_ActivatableWidgetBase> InWidget)
{
	if (!IsValid(InWorldContextObject) || InWidget.IsNull())
	{
		UE_LOG(LogYCNCommonUI, Warning, TEXT("InWorldContextObject 或者 InWidget 无效！"));
		return nullptr;
	}
	UYCNAsyncAction_PushWidget* Node = NewObject<UYCNAsyncAction_PushWidget>();
	if (Node)
	{
		Node->CacedWorldContextObject = InWorldContextObject;
		Node->CachedWidgetTag = InWidgetTag;
		Node->CachedWidget = InWidget;

		return Node;
	}
	return nullptr;
}
