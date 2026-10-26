// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "InputCoreTypes.h"

void UYCNWidget_EntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SetEnrtyDisplayText(CastChecked<UYCNListData_Base>(ListItemObject)->GetDataDisplayText());

}

void UYCNWidget_EntryBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	SwitchItemStatus(bIsSelected);
}

FReply UYCNWidget_EntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

	if (Border_HighLight)
	{
		Border_HighLight->SetVisibility(ESlateVisibility::Visible);
	}
	if (CommonText_DisplayText)
	{
		CommonText_DisplayText->SetStyle(TextStyle_Selected);
	}

	return FReply::Handled();
}

void UYCNWidget_EntryBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (UListView* ListView = Cast<UListView>(GetOwningListView()))
	{
		if (UObject* ListItem = GetListItem<UObject>())
		{
			ListView->SetSelectedItem(ListItem);
		}
	}

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		SetUserFocus(PlayerController);
	}

	if (HoverSound)
	{
		UGameplayStatics::PlaySound2D(this, HoverSound);
	}
	
	NativeOnHovered(true, IsListItemSelected());
}

void UYCNWidget_EntryBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	NativeOnHovered(false, IsListItemSelected());
}

void UYCNWidget_EntryBase::NativeOnHovered(bool bIsHovered, bool bIsSelected)
{
	BP_OnHovered(bIsHovered, bIsSelected);
}

void UYCNWidget_EntryBase::SetEnrtyDisplayText(const FText& InDisplayText)
{
	CommonText_DisplayText->SetText(InDisplayText);
}

void UYCNWidget_EntryBase::SwitchItemStatus(bool bIsSwitch)
{
	if (!Border_HighLight || !CommonText_DisplayText)return;

	if (bIsSwitch)
	{
		Border_HighLight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CommonText_DisplayText->SetStyle(TextStyle_Selected);
	}
	else
	{
		Border_HighLight->SetVisibility(ESlateVisibility::Collapsed);
		CommonText_DisplayText->SetStyle(TextStyle_Default);
	}
}
