// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/YCNWidget_ConfirmationWindows.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"

#include "YCNUIDemo/DebugHelper/YCNDebugHelper.h"

UYCNWindowInfo* UYCNWindowInfo::CreateOKButton(const FText& InWindowsDiaplayText, const FText& InWindowMessageText)
{
	UYCNWindowInfo* OkWindow = NewObject<UYCNWindowInfo>();
	if (OkWindow)
	{
		OkWindow->WindowDisplayText = InWindowsDiaplayText;
		OkWindow->WindowMessageText = InWindowMessageText;

		FWindowButtonInfo OKButtonInfo;
		OKButtonInfo.ButtonDisplayText = FText::FromString(TEXT("确认"));
		OKButtonInfo.ButtonType = EConfirmationWindowsButtonType::Yes;

		OkWindow->ButtonArray.Add(OKButtonInfo);

		return OkWindow;
	}
	return nullptr;
}

UYCNWindowInfo* UYCNWindowInfo::CreateYesNoButton(const FText& InWindowsDiaplayText, const FText& InWindowMessageText)
{
	UYCNWindowInfo* YesNoWindow = NewObject<UYCNWindowInfo>();
	if (YesNoWindow)
	{
		YesNoWindow->WindowDisplayText = InWindowsDiaplayText;
		YesNoWindow->WindowMessageText = InWindowMessageText;

		FWindowButtonInfo YesButtonInfo;
		YesButtonInfo.ButtonDisplayText = FText::FromString(TEXT("确认"));
		YesButtonInfo.ButtonType = EConfirmationWindowsButtonType::Yes;
		YesNoWindow->ButtonArray.Add(YesButtonInfo);

		FWindowButtonInfo NoButtonInfo;
		NoButtonInfo.ButtonDisplayText = FText::FromString(TEXT("取消"));
		NoButtonInfo.ButtonType = EConfirmationWindowsButtonType::No;

		YesNoWindow->ButtonArray.Add(NoButtonInfo);

		return YesNoWindow;
	}
	return nullptr;
}

void UYCNWidget_ConfirmationWindows::ButtonClicked(UYCNWindowInfo* InWindowInfo, TFunction<void(EConfirmationWindowsButtonType)> ButtonClickCallback)
{
	if (!InWindowInfo || !CommonText_WindowDisplayText || !CommonText_WindowMessageText || !EntryBox_Button)return;

	CommonText_WindowDisplayText->SetText(InWindowInfo->WindowDisplayText);
	CommonText_WindowMessageText->SetText(InWindowInfo->WindowMessageText);

	EntryBox_Button->Reset<UYCNWidget_ButtonBase>(
		[](UYCNWidget_ButtonBase& InButton)
		{
			InButton.OnClicked().Clear();
		});

	if (InWindowInfo->ButtonArray.IsEmpty())return;

	for (FWindowButtonInfo& CreatedButtonInfo : InWindowInfo->ButtonArray)
	{
		UYCNWidget_ButtonBase* CreatedButton = EntryBox_Button->CreateEntry<UYCNWidget_ButtonBase>();
		if (CreatedButton)
		{
			CreatedButton->CommonText_ButtonDisplayText->SetText(CreatedButtonInfo.ButtonDisplayText);
			CreatedButton->OnClicked().AddLambda(
				[this, ButtonClickCallback, CreatedButtonInfo]()
				{
					ButtonClickCallback(CreatedButtonInfo.ButtonType);
					DeactivateWidget();
				});
		}
	}
	if (EntryBox_Button->GetNumEntries() != 0)
	{
		EntryBox_Button->GetAllEntries().Last()->SetFocus();
	}
}
