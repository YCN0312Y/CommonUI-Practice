// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/Widgets/YCNWidget_ActivatableWidgetBase.h"
#include "YCNWidget_Loading.generated.h"

class UBorder;
class UCommonTextBlock;
class UProgressBar;
class UCommonNumericTextBlock;
class UYCNWidget_ButtonBase;
class UYCNStartupLoadingSubsystem;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class YCNUIDEMO_API UYCNWidget_Loading : public UYCNWidget_ActivatableWidgetBase
{
	GENERATED_BODY()

protected:
	//激活
	virtual void NativeOnInitialized()override;

	//激活
	virtual void NativeOnActivated()override;

	//停用
	virtual void NativeOnDeactivated()override;

	//销毁
	virtual void NativeDestruct()override;

private:
	//背景
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder>Border_Background;

	//标题
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>CommonText_Title;

	//描述
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>CommonText_Description;

	//进度条
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar>ProgressBar_Loading;

	//百分比
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonNumericTextBlock>CommonNumericText_Percent;

	//重试按键
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UYCNWidget_ButtonBase>CommonButton_Retry;

	UPROPERTY(Transient)
	TObjectPtr<UYCNStartupLoadingSubsystem>LoadingSubsystem;

	//完成
	bool bFinishing = false;

	FTimerHandle RefreshHandle;

private:
	//刷新加载
	void RefreshLoading();

	void Opclied_Retry();

	//清理刷新句柄
	void ClearRefreshHandle(FTimerHandle InHandle);
};
