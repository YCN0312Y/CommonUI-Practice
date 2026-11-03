// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_StringSwitch.h"
#include "YCNUIDemo/UI/ListData/YCNListData_String.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/UI/Components/YCNWidget_RotatorBase.h"

void UYCNWidget_Entry_StringSwitch::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (CommonButton_Previous)
    {
        CommonButton_Previous->OnClicked().AddUObject(this, &UYCNWidget_Entry_StringSwitch::OnClick_Previous);
    }
    if (CommonButton_Next)
    {
        CommonButton_Next->OnClicked().AddUObject(this, &UYCNWidget_Entry_StringSwitch::OnClick_Next);
    }
}

void UYCNWidget_Entry_StringSwitch::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    bool bOwningValueData = false;
    if (OwningValueData = Cast<UYCNListData_StringSwitch>(ListItemObject))
    {
        bOwningValueData = true;
    }
    else if (OwningValueData = Cast<UYCNListData_StringBool>(ListItemObject))
    {
        bOwningValueData = true;
    }

    if (bOwningValueData)
    {
        if (IsValid(OwningValueData) && IsValid(CommonRotator_SelectedText))
        {
            CommonRotator_SelectedText->PopulateTextLabels(OwningValueData->GetOptionList());
            RefreshEntryData();
        }
    }

}

void UYCNWidget_Entry_StringSwitch::SwitchItemStatus(bool bIsSwitch)
{
    Super::SwitchItemStatus(bIsSwitch);

    if (!IsValid(CommonButton_Previous) || !IsValid(CommonButton_Next) || !IsValid(CommonRotator_SelectedText))return;

    if (bIsSwitch)
    {
        CommonButton_Previous->SetButtonIconColor(CommonButton_Previous->IconSelectedColor);
        CommonButton_Next->SetButtonIconColor(CommonButton_Next->IconSelectedColor);
    }
    else
    {
        CommonButton_Previous->SetButtonIconColor(CommonButton_Previous->IconDefaultColor);
        CommonButton_Next->SetButtonIconColor(CommonButton_Next->IconDefaultColor);
    }
    CommonRotator_SelectedText->SetMyTextStyle(bIsSwitch, TextStyle_Default, TextStyle_Selected);
}

void UYCNWidget_Entry_StringSwitch::NativeOnEntryReleased()
{
    Super::NativeOnEntryReleased();
}

bool UYCNWidget_Entry_StringSwitch::IsValueModified() const
{
	return IsValid(OwningValueData) && OwningValueData->CanResetToDefault();
}

void UYCNWidget_Entry_StringSwitch::RefreshEntryData()
{
    UpdatePointStatus();
    if (!IsValid(CommonRotator_SelectedText) || !IsValid(OwningValueData)) return;

    const TArray<FText>& AllOption = OwningValueData->GetOptionList();
    if (AllOption.IsEmpty()) return;

    FText CurrentValue;

    if (!OwningValueData->GetSettingBinding().TryRead(CurrentValue) || CurrentValue.IsEmpty())
    {
        return;
    }

    const int32 FoundIndex = AllOption.IndexOfByPredicate(
        [&CurrentValue](const FText& Option) 
        { 
            return Option.EqualTo(CurrentValue); 
        });

    //找不到对应项就退回第一项
    CommonRotator_SelectedText->SetSelectedItem(FoundIndex != INDEX_NONE ? FoundIndex : 0);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *CommonRotator_SelectedText->GetSelectedText().ToString());
}

void UYCNWidget_Entry_StringSwitch::OnClick_Previous()
{
    SwitchText(-1);
}

void UYCNWidget_Entry_StringSwitch::OnClick_Next()
{
    SwitchText(1);
}

void UYCNWidget_Entry_StringSwitch::SwitchText(int32 InIndex)
{
    if (!IsValid(CommonRotator_SelectedText) || !IsValid(OwningValueData))return;

    const TArray<FText>& AllOption = OwningValueData->GetOptionList();

    if (AllOption.IsEmpty()) return;

    const int32 CurrentSelectedIndex = CommonRotator_SelectedText->GetSelectedIndex();

    //计算预测索引
    const int32 Num = AllOption.Num();

    int32 PredictiveSelectionIndex = (CurrentSelectedIndex + InIndex) % Num;

    // 处理负数取模
    if (PredictiveSelectionIndex < 0)
    {
        PredictiveSelectionIndex += Num;
        
    }
    if (OwningValueData->GetSettingBinding().TryWrite(AllOption[PredictiveSelectionIndex]))
    {
        RefreshEntryData();
    }
    else
    {
        CommonRotator_SelectedText->SetSelectedItem(PredictiveSelectionIndex);
    }
    
}

