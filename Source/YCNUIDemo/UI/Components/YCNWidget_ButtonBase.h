// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "YCNWidget_ButtonBase.generated.h"

class UCommonTextBlock;
class UCommonLazyImage;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct()override;
	//更新字体样式
	virtual void NativeOnCurrentTextStyleChanged()override;
	
public:
	// 按钮显示文本
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock>CommonText_ButtonDisplayText;
	// 按钮显示图标
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage>LazyImage_ButtonDisplayIcon;

private:
	//按钮显示文本
	UPROPERTY(EditAnywhere, Category = "YCN Button")
	FText ButtonDisplayText;

public:
	void SetButtonDisplayText(const FText& InButtonDisplayText);

	void SetButtonDisplayIcon(const TSoftObjectPtr<UTexture2D>& InButtonDisplayIcon);
};
