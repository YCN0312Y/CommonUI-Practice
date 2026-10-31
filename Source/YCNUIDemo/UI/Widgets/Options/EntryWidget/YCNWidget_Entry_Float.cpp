// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Float.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Float.h"

void UYCNWidget_Entry_Float::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_Float>(ListItemObject);
}
