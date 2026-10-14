// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "YCNWidget_MainStack.generated.h"

class UCommonActivatableWidgetStack;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_MainStack : public UCommonUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetStack*>RegisterStackList;

public:
	//添加控件到堆栈列表
	UFUNCTION(BlueprintCallable, Category = "YCNWidget_MainStack")
	void AddWidgetToStackList(const FGameplayTag& InStackTag, UCommonActivatableWidgetStack* InWidget);
	//根据标签获取软堆栈
	UFUNCTION(BlueprintCallable, Category = "YCNWidget_MainStack")
	UCommonActivatableWidgetStack* FindWidgetByTag(const FGameplayTag& InStackTag);
};
