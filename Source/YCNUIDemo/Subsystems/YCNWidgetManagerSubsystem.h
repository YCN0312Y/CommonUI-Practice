// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ConfirmationWindows.h"
#include "YCNWidgetManagerSubsystem.generated.h"

class UYCNWidget_ActivatableWidgetBase;
class UYCNWidget_MainStack;

UCLASS()
class YCNUIDEMO_API UYCNWidgetManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "YCNWidgetManagerSubsystem", meta = (WorldContext = "WorldContextObject"))
	static UYCNWidgetManagerSubsystem* Get(const UObject* WorldContextObject);
	
private:
	UPROPERTY()
	TObjectPtr<UYCNWidget_MainStack> OwningMainStack;

	UPROPERTY(Transient)
	TObjectPtr<UYCNWindowInfo> PushWindowInfo;

public:
	//注册主堆栈
	UFUNCTION(BlueprintCallable, Category = "YCNWidgetManagerSubsystem")
	void RegisterMainStack(UYCNWidget_MainStack* InMainStack);
	//推送控件到堆栈
	UFUNCTION(BlueprintCallable, Category = "YCNWidgetManagerSubsystem")
	void PushSoftWidgetToStack(UPARAM(meta = (Categories = "YCN.WidgetStack")) FGameplayTag InStackTag, TSoftClassPtr<UYCNWidget_ActivatableWidgetBase> InPushWidget);
	//推送确认按钮到堆栈
	void PushConfirmationToStack(
		EConfirmationWindowsType InWindowsType,
		const FText& InWindowDisplayText,
		const FText& InWinodwMessageText,
		TFunction<void(EConfirmationWindowsButtonType)>ButtonClickCallback
		);

};
