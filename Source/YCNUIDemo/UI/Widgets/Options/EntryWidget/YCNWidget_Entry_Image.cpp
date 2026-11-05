// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Image.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"
#include "CommonLazyImage.h"

void UYCNWidget_Entry_Image::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UYCNWidget_Entry_Image::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_Image>(ListItemObject);

	if (IsValid(CommonImage_Icon) && IsValid(OwningValueData))
	{
		UE_LOG(LogTemp, Warning, TEXT("1"));
		if (OwningValueData->GetDataIcon())
		{
			UE_LOG(LogTemp, Warning, TEXT("2"));
			CommonImage_Icon->SetBrushFromLazyTexture(OwningValueData->GetDataIcon());
		}
	}
}

void UYCNWidget_Entry_Image::SwitchItemStatus(bool bIsSwitch)
{
	Super::SwitchItemStatus(bIsSwitch);
}

void UYCNWidget_Entry_Image::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();
}

FReply UYCNWidget_Entry_Image::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UYCNWidget_Entry_Image::RefreshEntryData()
{
	Super::RefreshEntryData();
}
