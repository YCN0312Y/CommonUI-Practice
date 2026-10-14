// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/YCNWidget_MainStack.h"

void UYCNWidget_MainStack::AddWidgetToStackList(const FGameplayTag& InStackTag, UCommonActivatableWidgetStack* InWidget)
{
	if (!RegisterStackList.Contains(InStackTag))
	{
		RegisterStackList.Add(InStackTag, InWidget);
	}
}

UCommonActivatableWidgetStack* UYCNWidget_MainStack::FindWidgetByTag(const FGameplayTag& InStackTag)
{
	if (RegisterStackList.Contains(InStackTag))
	{
		return RegisterStackList.FindRef(InStackTag);
	}

	return nullptr;
}