// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "YCNWidget_RotatorBase.generated.h"

class UCommonTextStyle;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_RotatorBase : public UCommonRotator
{
	GENERATED_BODY()
	
public:
	void ShowUnlistedValue(const FText& Value);
	void SetMyTextStyle(bool bIsSelected, TSubclassOf<UCommonTextStyle> InDefaultStyle, TSubclassOf<UCommonTextStyle> InSelectedStyle);
};
