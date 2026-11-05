// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Components/YCNWidget_RotatorBase.h"
#include "CommonTextBlock.h"

void UYCNWidget_RotatorBase::ShowUnlistedValue(const FText& Value)
{
	SelectedIndex = INDEX_NONE;
	MyText->SetText(Value);
}

void UYCNWidget_RotatorBase::SetMyTextStyle(bool bIsSelected, TSubclassOf<UCommonTextStyle> InDefaultStyle, TSubclassOf<UCommonTextStyle> InSelectedStyle)
{
	if (!IsValid(InDefaultStyle) || !IsValid(InSelectedStyle))return;

	MyText->SetStyle(bIsSelected ? InSelectedStyle : InDefaultStyle);
}
