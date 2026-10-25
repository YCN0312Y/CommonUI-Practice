// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Components/YCNWidget_TabListBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Tab.h"
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

void UYCNWidget_TabListBase::ApplyRegisterTab(const UYCNListData_Tab* InData)
{
	if (!VerticalBox_TabContainer || !WidgetTree)return;

	RegisterTab(InData->GetDataID(), TabButton, nullptr);
	UYCNWidget_ButtonBase* CreatedTabButton = Cast<UYCNWidget_ButtonBase>(GetTabButtonBaseByID(InData->GetDataID()));
	if (CreatedTabButton)
	{
		if (MainTabIcon.Contains(InData->GetDataID()))
		{
			CreatedTabButton->SetButtonDisplayIcon(MainTabIcon.FindRef(InData->GetDataID()));
			CreatedTabButton->SetButtonDisplayText(InData->GetDataDisplayText());
			USizeBox* ButtonBox = WidgetTree->ConstructWidget<USizeBox>();
			if (ButtonBox)
			{
				ButtonBox->SetHeightOverride(50.f);
				ButtonBox->SetWidthOverride(200.f);

				USizeBoxSlot* ButtonBoxSlot = Cast<USizeBoxSlot>(ButtonBox->AddChild(CreatedTabButton));
				if (ButtonBoxSlot)
				{
					ButtonBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					ButtonBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
					ButtonBoxSlot->SetPadding(FMargin(0.f, 10.f, 0.f, 0.f));
				}
				VerticalBox_TabContainer->AddChild(ButtonBox);
			}
			
		}
	}
}
