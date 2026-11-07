// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/Settings/YCNDeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "YCNUIDemo/DataAsset/YCNWidgetDataAsset.h"
#include "Engine/AssetManager.h"
#include "CommonInputBaseTypes.h"

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

TSoftObjectPtr<UTexture2D> UYCNFunctionLibrary::GetAssetImageById(const FName& InDataID)
{
	if (InDataID.IsNone()) return nullptr;

	const UYCNDeveloperSettings* Settings = GetDefault<UYCNDeveloperSettings>();
	UYCNWidgetDataAsset* DataAsset = Settings->YCNDataAsset.LoadSynchronous();
	if (!DataAsset) return nullptr;

	return DataAsset->FindImageByDataID(InDataID);
}

bool UYCNFunctionLibrary::TryGetKeyBrush(const FKey& InKey, FSlateBrush& OutBrush)
{
	//重置
	OutBrush = FSlateBrush();

	//如果当前按键无效且是游戏手柄就返回false
	if (!InKey.IsValid() || InKey.IsGamepadKey()) return false;
	
	//Common输入平台设置
	const UCommonInputPlatformSettings* PlatformSettings = UCommonInputPlatformSettings::Get();
	if (!PlatformSettings) return false;

	const bool bFound = PlatformSettings->TryGetInputBrush(OutBrush, InKey, ECommonInputType::MouseAndKeyboard, NAME_None);

	//成功找到对应键且这个键的图标不为空
	return bFound && OutBrush.GetResourceObject() != nullptr;
}
