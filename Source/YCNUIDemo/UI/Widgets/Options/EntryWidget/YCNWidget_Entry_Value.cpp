// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Value.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Tags/YCNGameplayTags.h"
#include "CommonTextBlock.h"

void UYCNWidget_Entry_Value::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CommonButton_DropdownButton)
	{
		CommonButton_DropdownButton->OnClicked().AddUObject(this, &UYCNWidget_Entry_Value::OnClicked_DropdownButton);
	}
}

void UYCNWidget_Entry_Value::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_Value>(ListItemObject);
}

void UYCNWidget_Entry_Value::OnClicked_DropdownButton()
{
	UYCNWidgetManagerSubsystem* Manager = UYCNWidgetManagerSubsystem::Get(this);
	if (IsValid(Manager))
	{

	}
	Manager->PushSoftWidgetToStack(YCNGameplayTags::YCN_WidgetStack_Modal, DropdownOptionClass)
}
