// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Image.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Image.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
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

	if (!IsValid(CommonImage_Icon) || !IsValid(OwningValueData) || !IsValid(CommonText_ClickText) || !IsValid(CommonImage_ClickedIcon))return;

	//设置图标
	CommonImage_Icon->SetBrush(FSlateBrush());
	CommonImage_Icon->SetVisibility(ESlateVisibility::Hidden);

	if (OwningValueData->IsWaitingForInput())
	{
		CommonText_ClickText->SetStyle(TextStyle_Default);
		CommonText_ClickText->SetText(FText::FromString(TEXT("输入按键")));
		CommonText_ClickText->SetVisibility(ESlateVisibility::HitTestInvisible);
		CommonImage_ClickedIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		return;
	}
	else
	{
		CommonText_ClickText->SetVisibility(ESlateVisibility::Collapsed);
		CommonImage_ClickedIcon->SetVisibility(ESlateVisibility::Hidden);
	}

	if (OwningValueData->HasPreviewKey())
	{
		FSlateBrush KeyBrush;
		//显示按键图标
		if (UYCNFunctionLibrary::TryGetKeyBrush(OwningValueData->GetPreviewKey(), KeyBrush))
		{
			CommonImage_Icon->SetBrush(KeyBrush);
			CommonImage_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		return;
	}

	//普通图片条目
	const TSoftObjectPtr<UTexture2D> DataIcon = OwningValueData->GetDataIcon();
	if (!DataIcon.IsNull())
	{
		CommonImage_Icon->SetBrushFromLazyTexture(DataIcon);
		CommonImage_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UYCNWidget_Entry_Image::SetClickTextStyleAndText()
{
	if (!IsValid(CommonText_ClickText) || !IsValid(TextStyle_Error))return;
	CommonText_ClickText->SetStyle(TextStyle_Error);
	CommonText_ClickText->SetText(FText::FromString(TEXT("该按键已应用")));
	CommonText_ClickText->SetVisibility(ESlateVisibility::HitTestInvisible);
}
