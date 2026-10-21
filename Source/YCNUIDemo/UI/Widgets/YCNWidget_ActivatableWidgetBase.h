// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "YCNWidget_ActivatableWidgetBase.generated.h"

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_ActivatableWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "YCN PC")
	APlayerController* GetOwningPC();

    virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

protected:
    //控件输入模式
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCN|Input")
    ECommonInputMode WidgetInputMode = ECommonInputMode::Menu;
    //游戏鼠标捕获模式
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCN|Input")
    EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;
};
