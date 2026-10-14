// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/Settings/YCNDeveloperSettings.h"
#include "GameplayTagContainer.h"

TSoftClassPtr<UYCNWidget_ActivatableWidgetBase> UYCNFunctionLibrary::GetSoftWidgetByTag(FGameplayTag InWidgetTag)
{
	const UYCNDeveloperSettings* Settings = GetDefault<UYCNDeveloperSettings>();

	if (Settings && Settings->SoftWidgetRef.Contains(InWidgetTag))
	{
		return Settings->SoftWidgetRef.FindRef(InWidgetTag);
	}

	return TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>();
}
