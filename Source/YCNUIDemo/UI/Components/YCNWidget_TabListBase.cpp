// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "CommonActionWidget.h"

void UYCNWidget_TabListBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UYCNWidget_TabListBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (CommonAction_Previous && !PreviousTabInputActionData.IsNull())
	{
		CommonAction_Previous->SetInputAction(PreviousTabInputActionData);
	}
	if (CommonAction_Next && !NextTabInputActionData.IsNull())
	{
		CommonAction_Next->SetInputAction(NextTabInputActionData);
	}
}

void UYCNWidget_TabListBase::NativeDestruct()
{
	Super::NativeDestruct();

	if (VerticalBox_TabContainer)
	{
		VerticalBox_TabContainer->ClearChildren();
	}
}

void UYCNWidget_TabListBase::ApplyRegisterTab(const FName& InDataID)
{
	if (!VerticalBox_TabContainer || !WidgetTree)return;

	RegisterTab(InDataID, TabButton, nullptr);
	UYCNWidget_ButtonBase* CreatedTabButton = Cast<UYCNWidget_ButtonBase>(GetTabButtonBaseByID(InDataID));
	if (CreatedTabButton)
	{
		if (MainTabIcon.Contains(InDataID))
		{
			CreatedTabButton->SetButtonDisplayIcon(MainTabIcon.FindRef(InDataID));
			USizeBox* ButtonBox = WidgetTree->ConstructWidget<USizeBox>();
			if (ButtonBox)
			{
				ButtonBox->SetHeightOverride(100.f);
				ButtonBox->SetWidthOverride(100.f);

				USizeBoxSlot* ButtonBoxSlot = Cast<USizeBoxSlot>(ButtonBox);
				if (ButtonBoxSlot)
				{
					ButtonBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					ButtonBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				}
				ButtonBox->AddChild(CreatedTabButton);
				VerticalBox_TabContainer->AddChild(ButtonBox);
			}
			
		}
	}
}
