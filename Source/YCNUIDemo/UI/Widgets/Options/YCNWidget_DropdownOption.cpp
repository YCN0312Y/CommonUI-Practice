// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_DropdownOption.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "Components/ScrollBox.h"
#include "CommonTextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UYCNWidget_DropdownOption::SetOptionListData(UYCNListData_Value* InData)
{
	OptionListData = InData;

	AddDataToOptionList();
}

void UYCNWidget_DropdownOption::AddDataToOptionList()
{
	if (!IsValid(ScrollBox_OptionList) || !OptionButton || !IsValid(OptionListData)) return;
	
	ScrollBox_OptionList->ClearChildren();

	for (const FValueDataList& OptionData : OptionListData->GetOptionList())
	{
		const FText& OptionText = OptionData.OptionText;
		if (GetOwningPlayer())
		{
			UYCNWidget_ButtonBase* CreatedButton = CreateWidget<UYCNWidget_ButtonBase>(GetOwningPlayer(), OptionButton.Get());
			if (CreatedButton)
			{
				CreatedButton->SetButtonDisplayText(OptionText);
				CreatedButton->OnClicked().AddUObject(this, &UYCNWidget_DropdownOption::OnClick_CreatedButton, OptionText);
				ScrollBox_OptionList->AddChild(CreatedButton);
			}
		}
	}
}

void UYCNWidget_DropdownOption::OnClick_CreatedButton(FText InOptionText)
{
	OnUpdateSelectedText.Broadcast(InOptionText);
}

