// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Subsystems/YCNStartupLoadingSubsystem.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/Settings/YCNDeveloperSettings.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "YCNUIDemo/Tags/YCNGameplayTags.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_MainStack.h"

UYCNStartupLoadingSubsystem* UYCNStartupLoadingSubsystem::Get(const UObject* WorldContextObject)
{
	if (!GEngine)return nullptr;

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (World)
	{
		return World->GetGameInstance()->GetSubsystem<UYCNStartupLoadingSubsystem>();
	}
	return nullptr;
}

void UYCNStartupLoadingSubsystem::StartLoading()
{
	// 防止重复发起请求。
	if (bLoading || bReady)return;


	// 失败后重试，清理上一次的请求。
	if (LoadHandle.IsValid())
	{
		LoadHandle->CancelHandle();
		LoadHandle.Reset();
	}

	RequestedPaths.Reset();
	ErrorMessage.Reset();
	bReady = false;

	// 使用项目已有的 Widget 配置。
	const auto AnyKeyClass = UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_AnyKey);

	const auto MainMenuClass = UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_MainMenu);

	if (AnyKeyClass.IsNull() || MainMenuClass.IsNull())
	{
		Fail(TEXT("AnyKey or MainMenu is not configured."));
		return;
	}

	RequestedPaths.AddUnique(AnyKeyClass.ToSoftObjectPath());
	RequestedPaths.AddUnique(MainMenuClass.ToSoftObjectPath());

	const UYCNDeveloperSettings* Settings = GetDefault<UYCNDeveloperSettings>();

	for (const TSoftObjectPtr<UObject>& Asset : Settings->ExtraStartupAssets)
	{
		if (Asset.IsNull())
		{
			Fail(TEXT("ExtraStartupAssets contains an empty entry."));
			return;
		}

		RequestedPaths.AddUnique(Asset.ToSoftObjectPath());
	}

	bLoading = true;

	LoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(RequestedPaths,
		FStreamableDelegate::CreateUObject(this, &UYCNStartupLoadingSubsystem::OnAssetsLoaded));

	if (!LoadHandle.IsValid())
	{
		Fail(TEXT("Failed to create startup loading request."));
	}
}

void UYCNStartupLoadingSubsystem::OnAssetsLoaded()
{
	// 完成回调触发，不代表所有目标都成功加载。
	// 必须逐个检查。
	for (const FSoftObjectPath& Path : RequestedPaths)
	{
		if (!IsValid(Path.ResolveObject()))
		{
			Fail(FString::Printf(TEXT("Failed to load: %s"), *Path.ToString()));

			return;
		}
	}

	// 进一步检查两个必需的 Widget 类。
	const auto AnyKeyClass = UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_AnyKey);

	const auto MainMenuClass = UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_MainMenu);

	UClass* AnyKey = AnyKeyClass.Get();
	UClass* MainMenu = MainMenuClass.Get();

	if (!IsValid(AnyKey) || !IsValid(MainMenu) || !AnyKey->IsChildOf(UYCNWidget_ActivatableWidgetBase::StaticClass()) || !MainMenu->IsChildOf(
		UYCNWidget_ActivatableWidgetBase::StaticClass()) || AnyKey->HasAnyClassFlags(CLASS_Abstract) || MainMenu->HasAnyClassFlags(CLASS_Abstract))
	{
		Fail(TEXT("Startup widget classes are invalid."));
		return;
	}

	bLoading = false;
	bReady = true;

	// 此处不要释放 LoadHandle。
	// MainMenu 尚未创建，仍需要保留预加载资源。
}

float UYCNStartupLoadingSubsystem::GetLoadProgress() const
{
	if (bReady)return 1.0f;


	if (LoadHandle.IsValid())
	{
		return FMath::Clamp(LoadHandle->GetProgress(), 0.0f, 1.0f);
	}

	return 0.0f;
}

bool UYCNStartupLoadingSubsystem::ShowAnyKey()
{
	if (!bReady) return false;

	UYCNWidgetManagerSubsystem* WidgetManager = UYCNWidgetManagerSubsystem::Get(this);

	UYCNWidget_MainStack* MainStack = IsValid(WidgetManager) ? WidgetManager->GetOwningMainStack() : nullptr;

	if (!IsValid(MainStack))
	{
		Fail(TEXT("MainStack is not ready."));
		return false;
	}

	UCommonActivatableWidgetStack* MenuStack = MainStack->FindWidgetByTag(YCNGameplayTags::YCN_WidgetStack_Menu);

	if (!IsValid(MenuStack))
	{
		Fail(TEXT("Menu stack is not registered."));
		return false;
	}

	const auto SoftClass = UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_AnyKey);

	UClass* WidgetClass = SoftClass.Get();

	if (!IsValid(WidgetClass))
	{
		Fail(TEXT("AnyKey class is unavailable."));
		return false;
	}

	// 类已经加载成功，此处只创建实例。
	UYCNWidget_ActivatableWidgetBase* Widget = MenuStack->AddWidget<UYCNWidget_ActivatableWidgetBase>(WidgetClass);

	if (!IsValid(Widget))
	{
		Fail(TEXT("Failed to create AnyKey widget."));
		return false;
	}

	return true;
}

void UYCNStartupLoadingSubsystem::Fail(const FString& Message)
{
	bLoading = false;
	bReady = false;
	ErrorMessage = Message;

	UE_LOG(LogTemp, Error, TEXT("[Startup] %s"), *Message);
}

void UYCNStartupLoadingSubsystem::Deinitialize()
{
	if (LoadHandle.IsValid())
	{
		LoadHandle->CancelHandle();
		LoadHandle.Reset();
	}

	RequestedPaths.Reset();
	bLoading = false;
	bReady = false;

	Super::Deinitialize();
}