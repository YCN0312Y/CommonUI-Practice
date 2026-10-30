// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ConfirmationWindows.h"
#include "YCNAsyncAction_PushConfirmation.generated.h"

class UYCNWidget_ActivatableWidgetBase;

UCLASS(meta = (HideThen = "true"))
class YCNUIDEMO_API UYCNAsyncAction_PushConfirmation : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Activate() override;

private:
	UPROPERTY()
	UObject* CacedWorldContextObject;
	UPROPERTY()
	FText CachedWindowMessageText;
	UPROPERTY()
	EConfirmationWindowsType CachedWindowsType;
	UPROPERTY()
	TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>CachedWidget;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, EConfirmationWindowsButtonType, ClickedButtonType);
	UPROPERTY(BlueprintAssignable, Category = "UYCNAsyncAction_PushWidget")
	FOnButtonClicked OnButtonClicked;

public:
	UFUNCTION(BlueprintCallable, Category = "UYCNAsyncAction_PushWidget", meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UYCNAsyncAction_PushConfirmation* PushConfrimationWindow(
		UObject* InWorldContextObject, 
		EConfirmationWindowsType InWindowsType,
		FText InMessageText,
		TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>InWidget);
};
