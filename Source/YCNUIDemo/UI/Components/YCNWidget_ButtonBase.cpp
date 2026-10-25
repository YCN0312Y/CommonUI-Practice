// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "Engine/Texture2D.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"

void UYCNWidget_ButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (CommonText_ButtonDisplayText)
	{
		CommonText_ButtonDisplayText->SetText(ButtonDisplayText);
	}
}

void UYCNWidget_ButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonText_ButtonDisplayText)
	{
		CommonText_ButtonDisplayText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UYCNWidget_ButtonBase::SetButtonDisplayText(const FText& InButtonDisplayText)
{
	ButtonDisplayText = InButtonDisplayText;
	if (CommonText_ButtonDisplayText)
	{
		CommonText_ButtonDisplayText->SetText(ButtonDisplayText);
	}
}

void UYCNWidget_ButtonBase::SetButtonDisplayIcon(const TSoftObjectPtr<UTexture2D>& InButtonDisplayIcon)
{
	if (LazyImage_ButtonDisplayIcon)
	{
		LazyImage_ButtonDisplayIcon->SetBrushFromLazyTexture(InButtonDisplayIcon);
	}
}

void UYCNWidget_ButtonBase::SetButtonIconColor(const FSlateColor& InIconColor)
{
	if (LazyImage_ButtonDisplayIcon)
	{
		LazyImage_ButtonDisplayIcon->SetBrushTintColor(InIconColor);
	}
}
