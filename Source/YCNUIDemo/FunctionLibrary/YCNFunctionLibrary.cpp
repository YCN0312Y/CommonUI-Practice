// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/Settings/YCNDeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "YCNUIDemo/DataAsset/YCNWidgetDataAsset.h"
#include "Engine/AssetManager.h"

TSoftClassPtr<UYCNWidget_ActivatableWidgetBase> UYCNFunctionLibrary::GetSoftWidgetByTag(FGameplayTag InWidgetTag)
{
	const UYCNDeveloperSettings* Settings = GetDefault<UYCNDeveloperSettings>();

	if (Settings && Settings->SoftWidgetRef.Contains(InWidgetTag))
	{
		return Settings->SoftWidgetRef.FindRef(InWidgetTag);
	}

	return TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>();
}

TSoftClassPtr<UYCNWidget_MainStack> UYCNFunctionLibrary::GetMainStackWidget()
{
	const UYCNDeveloperSettings* Settings = GetDefault<UYCNDeveloperSettings>();

	if (Settings && !Settings->MainStackWidget.IsNull())
	{
		return Settings->MainStackWidget;
	}

	return TSoftClassPtr<UYCNWidget_MainStack>();
}

UTexture2D* UYCNFunctionLibrary::GetAssetImageById(const FName& InDataID)
{
	if (InDataID.IsNone()) return nullptr;

	const UYCNDeveloperSettings* Settings = GetDefault<UYCNDeveloperSettings>();
	UYCNWidgetDataAsset* DataAsset = Settings->YCNDataAsset.LoadSynchronous();
	if (!DataAsset) return nullptr;

	return DataAsset->FindImageByDataID(InDataID).LoadSynchronous();
}
