// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_MainStack.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/Tags/YCNGameplayTags.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UYCNWidgetManagerSubsystem* UYCNWidgetManagerSubsystem::Get(const UObject* WorldContextObject)
{
	if (!GEngine)return nullptr;

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (World)
	{
		return World->GetGameInstance()->GetSubsystem<UYCNWidgetManagerSubsystem>();
	}
	return nullptr;
}

void UYCNWidgetManagerSubsystem::RegisterMainStack(UYCNWidget_MainStack* InMainStack)
{
	OwningMainStack = InMainStack;
}

void UYCNWidgetManagerSubsystem::PushSoftWidgetToStack(UPARAM(meta = (Categories = "YCN.WidgetStack")) FGameplayTag InStackTag, TSoftClassPtr<UYCNWidget_ActivatableWidgetBase> InPushWidget)
{
	if (InPushWidget.IsNull())return;

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		InPushWidget.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, InStackTag, InPushWidget]()
			{
				UClass* FoundWidget = InPushWidget.Get();
				if (FoundWidget)
				{
					UCommonActivatableWidgetStack* FoundWidgetStack = OwningMainStack->FindWidgetByTag(InStackTag);
					if (FoundWidgetStack)
					{
						FoundWidgetStack->AddWidget<UYCNWidget_ActivatableWidgetBase>(FoundWidget);
					}
				}
			})
	);
}

void UYCNWidgetManagerSubsystem::PushConfirmationToStack(EConfirmationWindowsType InWindowsType, const FText& InWindowDisplayText, const FText& InWinodwMessageText, TFunction<void(EConfirmationWindowsButtonType)> ButtonClickCallback)
{
	switch (InWindowsType)
	{
	case EConfirmationWindowsType::OK:
		PushWindowInfo = UYCNWindowInfo::CreateOKButton(InWindowDisplayText, InWinodwMessageText);
		break;
	case EConfirmationWindowsType::YesNO:
		PushWindowInfo = UYCNWindowInfo::CreateYesNoButton(InWindowDisplayText, InWinodwMessageText);
		break;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_Window).ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, ButtonClickCallback]()
			{
				UClass* FoundWidget = UYCNFunctionLibrary::GetSoftWidgetByTag(YCNGameplayTags::YCN_Widget_Window).Get();
				if (FoundWidget)
				{
					UCommonActivatableWidgetStack* FoundWidgetStack = OwningMainStack->FindWidgetByTag(YCNGameplayTags::YCN_WidgetStack_Modal);
					if (FoundWidgetStack)
					{
						UYCNWidget_ConfirmationWindows* CreatedWindow = FoundWidgetStack->AddWidget<UYCNWidget_ConfirmationWindows>(FoundWidget);
						if (CreatedWindow)
						{
							CreatedWindow->ButtonClicked(PushWindowInfo, ButtonClickCallback);
						}
					}
				}
			})
	);
}

void UYCNWidgetManagerSubsystem::PushDropdownOptions(TSoftClassPtr<UYCNWidget_DropdownOption> InDropdownOptionWidget, UYCNListData_Value* InData)
{
	if (!InDropdownOptionWidget.IsNull() || !IsValid(InData) || InData->GetOptionList().IsEmpty() || !OwningMainStack)return;

	TWeakObjectPtr<UYCNWidgetManagerSubsystem>WeakThis(this);
	TWeakObjectPtr<UYCNListData_Value>WeakData(InData);

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		InDropdownOptionWidget.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[WeakThis, WeakData, InDropdownOptionWidget]()
			{
				UYCNWidgetManagerSubsystem* Subsystem = WeakThis.Get();
				UYCNListData_Value* Data = WeakData.Get();
				UClass* DropdownOptionClass = InDropdownOptionWidget.Get();
				if (!Subsystem || IsValid(Subsystem->OwningMainStack))return;
				if (!IsValid(Data))return;
				if (!DropdownOptionClass)return;
				//UCommonActivatableWidgetStack* Subsystem->OwningMainStack->FindWidgetByTag(YCNGameplayTags::YCN_WidgetStack_Modal);
			})
	);
}

void UYCNWidgetManagerSubsystem::SetOwningMainStackBackground(bool bIsVisible)
{
	OwningMainStack->SetBackgroundVisible(bIsVisible);
}
