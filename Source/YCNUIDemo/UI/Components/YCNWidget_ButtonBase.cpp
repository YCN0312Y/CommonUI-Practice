// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "CommonTextBlock.h"

void UYCNWidget_ButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (CommonText_ButtonDisplayText)
	{
		CommonText_ButtonDisplayText->SetText(FText::FromString(ButtonDisplayText));
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
