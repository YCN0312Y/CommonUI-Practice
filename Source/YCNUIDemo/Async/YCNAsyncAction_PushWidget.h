// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "YCNAsyncAction_PushWidget.generated.h"

class UYCNWidget_ActivatableWidgetBase;

UCLASS()
class YCNUIDEMO_API UYCNAsyncAction_PushWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Activate() override;

private:
	UPROPERTY()
	UObject* CacedWorldContextObject;
	UPROPERTY()
	FGameplayTag CachedWidgetTag;
	UPROPERTY()
	TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>CachedWidget;

public:
	UFUNCTION(BlueprintCallable, Category = "UYCNAsyncAction_PushWidget", meta = (WorldContext = "InWorldContextObject"))
	static UYCNAsyncAction_PushWidget* PushSoftWidget(
		UObject* InWorldContextObject, UPARAM(meta = (Categories = "YCN.WidgetStack")) FGameplayTag InWidgetTag, TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>InWidget);
};
