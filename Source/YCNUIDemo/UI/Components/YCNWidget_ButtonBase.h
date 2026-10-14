// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "YCNWidget_ButtonBase.generated.h"

class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct()override;
	//更新字体样式
	virtual void NativeOnCurrentTextStyleChanged()override;
	
public:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>CommonText_ButtonDisplayText;

private:
	UPROPERTY(EditAnywhere, Category = "YCN Button")
	FString ButtonDisplayText;
};
