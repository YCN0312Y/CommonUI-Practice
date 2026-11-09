// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_None.h"
#include "YCNUIDemo/UI/ListData/YCNListData_None.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"

void UYCNWidget_Entry_None::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_None>(ListItemObject);
}

