// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "YCNDeveloperSettings.generated.h"

class UYCNWidget_ActivatableWidgetBase;
class UYCNWidget_MainStack;

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "YCN Common UI Settings"))
class YCNUIDEMO_API UYCNDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	//主堆栈控件
	UPROPERTY(Config, EditAnywhere, Category = "Widgets")
	TSoftClassPtr<UYCNWidget_MainStack>MainStackWidget;
	//需要推送的软控件引用
	UPROPERTY(Config, EditAnywhere, Category = "Widgets", meta = (ForceInlineRow, Categories = "YCN.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>>SoftWidgetRef;
};
