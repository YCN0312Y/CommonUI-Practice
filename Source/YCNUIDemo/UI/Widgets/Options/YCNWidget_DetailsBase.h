// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "YCNWidget_DetailsBase.generated.h"

class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_DetailsBase : public UCommonUserWidget
{
	GENERATED_BODY()
	
private:
	//标签显示文本
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>TextBlock_TabDisplayText;
	//描述文本
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonTextBlock>TextBlock_DescriptionText;

public:
	//设置数据详情
	void SetDataDetails(const FText& InTabDisplayText, const FText& InDescriptionText);
};
