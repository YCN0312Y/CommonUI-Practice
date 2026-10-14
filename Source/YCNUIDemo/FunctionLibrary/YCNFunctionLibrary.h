// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "YCNFunctionLibrary.generated.h"

class UYCNWidget_ActivatableWidgetBase;
UCLASS()
class YCNUIDEMO_API UYCNFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "YCNFunctionLibrary")
	static TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>GetSoftWidgetByTag(FGameplayTag InWidgetTag);
};
