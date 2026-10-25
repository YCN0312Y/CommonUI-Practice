// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "CommonTextBlock.h"

void UYCNWidget_EntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SetEnrtyDisplayText(CastChecked<UYCNListData_Base>(ListItemObject)->GetDataDisplayText());

}

void UYCNWidget_EntryBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (CommonText_DisplayText && HoverSound && TextStyle_Selected)
	{
		CommonText_DisplayText->SetStyle(TextStyle_Selected);

		UGameplayStatics::PlaySound2D(this, HoverSound);

		if (Border_HighLight && Border_HighLight->GetVisibility() == ESlateVisibility::Collapsed)
		{
			Border_HighLight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}

void UYCNWidget_EntryBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (CommonText_DisplayText && TextStyle_Default)
	{
		CommonText_DisplayText->SetStyle(TextStyle_Default);
	}

	if (Border_HighLight && Border_HighLight->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		Border_HighLight->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UYCNWidget_EntryBase::SetEnrtyDisplayText(const FText& InDisplayText)
{
	CommonText_DisplayText->SetText(InDisplayText);
}
