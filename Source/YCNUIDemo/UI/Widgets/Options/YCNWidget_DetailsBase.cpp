// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_DetailsBase.h"
#include "CommonTextBlock.h"

void UYCNWidget_DetailsBase::SetDataDetails(const FText& InTabDisplayText, const FText& InDescriptionText)
{
	if (!TextBlock_DescriptionText || !TextBlock_TabDisplayText)return;

	TextBlock_TabDisplayText->SetText(InTabDisplayText);
	TextBlock_DescriptionText->SetText(InDescriptionText);
}
