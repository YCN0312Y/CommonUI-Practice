// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_None.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Text.h"

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

void UYCNWidget_Entry_None::RefreshEntryData()
{
	Super::RefreshEntryData();
}
