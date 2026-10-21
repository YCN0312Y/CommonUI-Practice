// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"

APlayerController* UYCNWidget_ActivatableWidgetBase::GetOwningPC()
{
    return GetOwningPlayer<APlayerController>() ? GetOwningPlayer<APlayerController>() : nullptr;
}

TOptional<FUIInputConfig>UYCNWidget_ActivatableWidgetBase::GetDesiredInputConfig() const
{
    const bool bIsMenu = WidgetInputMode == ECommonInputMode::Menu;
    //如果当前输入模式为仅UI，那么就不捕获鼠标反之亦然
    const EMouseCaptureMode CaptureMode = bIsMenu ? EMouseCaptureMode::NoCapture : GameMouseCaptureMode;

    FUIInputConfig InputConfig(WidgetInputMode, CaptureMode);

    InputConfig.bIgnoreMoveInput = bIsMenu;
    InputConfig.bIgnoreLookInput = bIsMenu;

    return InputConfig;
}
