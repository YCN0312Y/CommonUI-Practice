// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNWidget_Options.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNWidget_Options : public UYCNWidget_ActivatableWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized()override;

private:
	// 重置
	UPROPERTY(EditAnywhere, Category = "YCN Action")
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

private:
	void OnClickedBack();
	void OnClickedReset();
};
