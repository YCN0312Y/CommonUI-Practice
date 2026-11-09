// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/YCNWidget_Loading.h"
#include "YCNUIDemo/Subsystems/YCNStartupLoadingSubsystem.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "TimerManager.h"
#include "CommonTextBlock.h"
#include "CommonNumericTextBlock.h"
#include "Components/ProgressBar.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

#include "YCNUIDemo/DebugHelper/YCNDebugHelper.h"

void UYCNWidget_Loading::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IsValid(CommonButton_Retry))
	{
		CommonButton_Retry->OnClicked().AddUObject(this, &UYCNWidget_Loading::Opclied_Retry);
	}
}

void UYCNWidget_Loading::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (IsValid(CommonButton_Retry))
	{
		CommonButton_Retry->SetButtonDisplayText(FText::FromString(TEXT("重试")));
	}
	LoadingSubsystem = UYCNStartupLoadingSubsystem::Get(this);
	
	bFinishing = false;

	if (IsValid(CommonText_Description))
	{
		CommonText_Description->SetText(FText::FromString(TEXT("正在加载启动资源……")));
	}

	if (IsValid(CommonButton_Retry))
	{
		CommonButton_Retry->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(LoadingSubsystem))
	{
		LoadingSubsystem->StartLoading();
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UYCNWidget_Loading::RefreshLoading, 0.05f, true);
		RefreshLoading();
	}

}

void UYCNWidget_Loading::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	ClearRefreshHandle(RefreshHandle);
}

void UYCNWidget_Loading::NativeDestruct()
{
	Super::NativeDestruct();

	ClearRefreshHandle(RefreshHandle);
}

void UYCNWidget_Loading::RefreshLoading()
{
	if (bFinishing || !IsValid(ProgressBar_Loading) || !IsValid(CommonText_Description) || !IsValid(CommonNumericText_Percent) || !IsValid(GetWorld()))return;

	if (!IsValid(LoadingSubsystem))
	{
		UE_LOG(LogYCNCommonUI, Error, TEXT("启动加载管理器无效!"));
		CommonText_Description->SetText(FText::FromString(TEXT("启动加载管理器无效")));
		ClearRefreshHandle(RefreshHandle);
		return;
	}

	const float ProgressValue = FMath::Clamp(LoadingSubsystem->GetLoadProgress(), 0.0f, 1.0f);
	ProgressBar_Loading->SetPercent(ProgressValue);
	CommonNumericText_Percent->SetNumericType(ECommonNumericType::Percentage);
	CommonNumericText_Percent->SetCurrentValue(ProgressValue);

	if (!LoadingSubsystem->ErrorMessage.IsEmpty())
	{
		CommonText_Description->SetText(FText::FromString(LoadingSubsystem->ErrorMessage));
		if (IsValid(CommonButton_Retry))
		{
			CommonButton_Retry->SetVisibility(ESlateVisibility::Visible);
			CommonButton_Retry->SetKeyboardFocus();
		}
		ClearRefreshHandle(RefreshHandle);
		return;
	}
	if (LoadingSubsystem->bReady)
	{
		CommonText_Description->SetText(FText::FromString(TEXT("加载完成")));

		bFinishing = true;
		if (LoadingSubsystem->ShowAnyKey())
		{
			ClearRefreshHandle(RefreshHandle);
			DeactivateWidget();
		}
		else
		{
			bFinishing = false;
			CommonText_Description->SetText(FText::FromString(LoadingSubsystem->ErrorMessage));
			if (IsValid(CommonButton_Retry))
			{
				CommonButton_Retry->SetVisibility(ESlateVisibility::Visible);
				CommonButton_Retry->SetKeyboardFocus();
			}
			ClearRefreshHandle(RefreshHandle);
		}
	}
}

void UYCNWidget_Loading::Opclied_Retry()
{
	if (!IsValid(CommonButton_Retry) || !IsValid(CommonText_Description) || !IsValid(LoadingSubsystem) || !IsValid(GetWorld()))return;

	ClearRefreshHandle(RefreshHandle);

	bFinishing = false;
	CommonButton_Retry->SetVisibility(ESlateVisibility::Collapsed);
	CommonText_Description->SetText(FText::FromString(TEXT("正在重新加载")));
	LoadingSubsystem->StartLoading();
	GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UYCNWidget_Loading::RefreshLoading, 0.05f, true);
	RefreshLoading();
}

void UYCNWidget_Loading::ClearRefreshHandle(FTimerHandle InHandle)
{
	if (GetWorld() && InHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InHandle);
	}
}
