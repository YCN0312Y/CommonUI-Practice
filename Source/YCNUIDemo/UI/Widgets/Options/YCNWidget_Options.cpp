// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_Options.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"

void UYCNWidget_Options::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ResetAction, true, FSimpleDelegate::CreateUObject(this, &UYCNWidget_Options::OnClickedReset)));
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(ICommonInputModule::GetSettings().GetDefaultBackAction(), 
			true, FSimpleDelegate::CreateUObject(this, &UYCNWidget_Options::OnClickedBack)));
}

void UYCNWidget_Options::OnClickedBack()
{

}

void UYCNWidget_Options::OnClickedReset()
{

}
