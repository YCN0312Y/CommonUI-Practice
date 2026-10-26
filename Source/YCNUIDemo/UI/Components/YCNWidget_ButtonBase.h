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

	//图标默认颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCN Button")
	FSlateColor IncoDefaultColor = FSlateColor(FColor::White);
	//图标选中颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "YCN Button")
	FSlateColor IncoSelectedColor = FSlateColor(FColor::Black);
private:
	//按钮显示文本
	UPROPERTY(EditAnywhere, Category = "YCN Button")
	FText ButtonDisplayText;


public:
	void SetButtonDisplayText(const FText& InButtonDisplayText);
	//设置按钮图标
	UFUNCTION(BlueprintCallable, Category = "YCN Button")
	void SetButtonDisplayIcon(const TSoftObjectPtr<UTexture2D>& InButtonDisplayIcon);
	//设置按钮图标颜色
	UFUNCTION(BlueprintCallable, Category = "YCN Button")
	void SetButtonIconColor(const FSlateColor& InIconColor);
};
