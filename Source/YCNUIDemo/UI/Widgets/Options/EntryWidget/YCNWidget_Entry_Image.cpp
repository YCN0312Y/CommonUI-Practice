// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Image.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"
#include "CommonLazyImage.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "Engine/Texture2D.h"

void UYCNWidget_Entry_Image::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UYCNWidget_Entry_Image::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_Image>(ListItemObject);

	RefreshEntryData();
}

void UYCNWidget_Entry_Image::SwitchItemStatus(bool bIsSwitch)
{
	Super::SwitchItemStatus(bIsSwitch);
}

void UYCNWidget_Entry_Image::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();

	OwningValueData = nullptr;

	RefreshEntryData();
}

FReply UYCNWidget_Entry_Image::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && IsValid(OwningValueData))
	{
		const TFunction<void()> Callback = OwningValueData->GetClickCallBack();

		if (Callback)
		{
			Callback();
			return FReply::Handled();
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UYCNWidget_Entry_Image::RefreshEntryData()
{
	Super::RefreshEntryData();

	if (!IsValid(CommonImage_Icon) || !IsValid(OwningValueData)) return;

	CommonImage_Icon->SetBrush(FSlateBrush());
	CommonImage_Icon->SetVisibility(ESlateVisibility::Hidden);

	//KeyData
	if (OwningValueData->HasPreviewKey())
	{
		FSlateBrush KeyBrush;

		if (UYCNFunctionLibrary::TryGetKeyBrush(OwningValueData->GetPreviewKey(), KeyBrush))
		{
			CommonImage_Icon->SetBrush(KeyBrush);
			CommonImage_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		return;
	}

	//普通的ImageData
	TSoftObjectPtr<UTexture2D>DataIcon = OwningValueData->GetDataIcon();
	if (!DataIcon.IsNull())
	{
		CommonImage_Icon->SetBrushFromLazyTexture(DataIcon);
		CommonImage_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
