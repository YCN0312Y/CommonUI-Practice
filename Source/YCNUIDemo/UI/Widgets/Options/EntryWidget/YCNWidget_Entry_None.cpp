// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_None.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Text.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"

void UYCNWidget_Entry_None::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UYCNWidget_Entry_None::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_Text>(ListItemObject);
}

void UYCNWidget_Entry_None::SwitchItemStatus(bool bIsSwitch)
{
	Super::SwitchItemStatus(bIsSwitch);
}

void UYCNWidget_Entry_None::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();
}

FReply UYCNWidget_Entry_None::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsValid(OwningValueData))return FReply::Handled();

	if (UYCNWidgetManagerSubsystem* Manager = UYCNWidgetManagerSubsystem::Get(this))
	{
		Manager->PushConfirmationToStack(
			OwningValueData->GetClickedWindowsType(),
			OwningValueData->GetWindowsMessageText(),
			OwningValueData->GetClickCallBack());
	}
	return FReply::Handled();
}

void UYCNWidget_Entry_None::RefreshEntryData()
{
	Super::RefreshEntryData();
}
