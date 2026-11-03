// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "InputCoreTypes.h"

void UYCNWidget_EntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (const UYCNListData_Base* Data = Cast<UYCNListData_Base>(ListItemObject))
	{
		SetEnrtyDisplayText(Data->GetDataDisplayText());
	}
	else
	{
		SetEnrtyDisplayText(FText::GetEmpty());
	}

	SwitchItemStatus(IsListItemSelected());

	if (!UserSettings.IsValid())
	{
		UserSettings = UYCNGameUserSettings::Get();
	}
}

void UYCNWidget_EntryBase::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	SwitchItemStatus(bIsSelected);
}

void UYCNWidget_EntryBase::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	//清除本次使用留下的视觉状态
	SwitchItemStatus(false);
}

FReply UYCNWidget_EntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

	// 焦点变化不直接制造另一份“选中状态”
	SwitchItemStatus(IsListItemSelected());

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

	if (HoverSound)
	{
		UGameplayStatics::PlaySound2D(this, HoverSound);
	}

}

void UYCNWidget_EntryBase::UpdatePointStatus()
{
	if (!IsValid(Border_Point)) return;

	Border_Point->SetVisibility(IsValueModified() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UYCNWidget_EntryBase::SetEnrtyDisplayText(const FText& InDisplayText)
{
	CommonText_DisplayText->SetText(InDisplayText);
}

void UYCNWidget_EntryBase::SwitchItemStatus(bool bIsSwitch)
{
	if (!IsValid(Border_Point) || !IsValid(Border_HighLight) || !IsValid(CommonText_DisplayText))return;

	if (IsValid(Border_Point))
	{
		Border_Point->SetBrushColor(bIsSwitch ? FLinearColor(FColor::Black) : FLinearColor(FColor::White));
	}
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
