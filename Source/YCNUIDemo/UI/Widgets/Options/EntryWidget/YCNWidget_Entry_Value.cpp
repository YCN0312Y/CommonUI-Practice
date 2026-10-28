// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Value.h"
#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_DropdownOption.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
#include "YCNUIDemo/Settings/YCNGameUserSettings.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Value.h"
#include "YCNUIDemo/Tags/YCNGameplayTags.h"
#include "CommonTextBlock.h"
#include "Components/MenuAnchor.h"

void UYCNWidget_Entry_Value::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CommonButton_DropdownButton)
	{
		CommonButton_DropdownButton->OnClicked().AddUObject(this, &UYCNWidget_Entry_Value::OnClicked_DropdownButton);
	}

    if (MenuAnchor_Option)
    {
        MenuAnchor_Option->OnGetUserMenuContentEvent.BindDynamic(this, &UYCNWidget_Entry_Value::CreateOptionMenuContent);
        MenuAnchor_Option->OnMenuOpenChanged.AddDynamic(this, &UYCNWidget_Entry_Value::OnOpen_MenuAnchorOption);
    }
}

void UYCNWidget_Entry_Value::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_Value>(ListItemObject);

    if (!IsValid(CommonText_SelectedText)) return;
    
    CommonText_SelectedText->SetText(FText::GetEmpty());

    if (IsValid(OwningValueData) && OwningValueData->GetOptionList().IsEmpty())
    {
        CommonText_SelectedText->SetText(FText::FromString(TEXT("无效的选项")));
        return;
    }

    FText CurrentValue;

    if (!OwningValueData->TryReadSettingText(CurrentValue) || CurrentValue.IsEmpty())
    {
        CurrentValue = OwningValueData->GetOwningDefaultValue();
    }

    CommonText_SelectedText->SetText(CurrentValue);

}

FReply UYCNWidget_Entry_Value::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UYCNWidget_Entry_Value::SwitchItemStatus(bool bIsSwitch)
{
    Super::SwitchItemStatus(bIsSwitch);

    if (!CommonText_SelectedText || !CommonButton_DropdownButton)return;

    if (bIsSwitch)
    {
        CommonText_SelectedText->SetStyle(TextStyle_Selected);
        CommonButton_DropdownButton->SetButtonIconColor(CommonButton_DropdownButton->IncoSelectedColor);
    }
    else
    {
        CommonText_SelectedText->SetStyle(TextStyle_Default);
        CommonButton_DropdownButton->SetButtonIconColor(CommonButton_DropdownButton->IncoDefaultColor);
    }
}

void UYCNWidget_Entry_Value::NativeOnEntryReleased()
{
    CloseDropdownMenu(false);
    OnDropdownOpenChanged.Unbind();

    Super::NativeOnEntryReleased();
}

void UYCNWidget_Entry_Value::OnClicked_DropdownButton()
{
    if (!MenuAnchor_Option || MenuAnchor_Option->IsOpen() || OwningValueData->GetOptionList().IsEmpty()) return;
    
    MenuAnchor_Option->Open(true);

    if (MenuAnchor_Option->IsOpen() && CurrentDropdownMenu.IsValid())
    {
        CurrentDropdownMenu->SetFocus();
    }
}

UUserWidget* UYCNWidget_Entry_Value::CreateOptionMenuContent()
{
    if (!IsValid(MenuAnchor_Option) || !IsValid(OwningValueData)) return nullptr;
    
    UClass* WidgetClass = MenuAnchor_Option->MenuClass.Get();

    if (!WidgetClass || !WidgetClass->IsChildOf(UYCNWidget_DropdownOption::StaticClass())) return nullptr;
    
    UYCNWidget_DropdownOption* MenuWidget = CreateWidget<UYCNWidget_DropdownOption>(GetOwningPlayer(), WidgetClass);
    
    if (MenuWidget)
    {
        CurrentDropdownMenu = MenuWidget;
        MenuWidget->OnUpdateSelectedText.AddUObject(this, &UYCNWidget_Entry_Value::OnClick_ListCreatedButton);
        MenuWidget->SetOptionListData(OwningValueData);
    }

    return MenuWidget;
}

void UYCNWidget_Entry_Value::OnOpen_MenuAnchorOption(bool bIsOpen)
{
    if (!bIsOpen)
    {
        CurrentDropdownMenu.Reset();
    }

    OnDropdownOpenChanged.ExecuteIfBound(bIsOpen);
}

void UYCNWidget_Entry_Value::OnClick_ListCreatedButton(FText InSelectedText)
{
    if (!CommonText_SelectedText)return;
    CommonText_SelectedText->SetText(InSelectedText);

    if (!IsValid(OwningValueData)) return;
    
    CloseDropdownMenu();

    const bool bHasWriteBinding = OwningValueData->TryWriteSettingText(InSelectedText);

    if (bHasWriteBinding && IsValid(CommonText_SelectedText))
    {
        CommonText_SelectedText->SetText(InSelectedText);
    }

}

void UYCNWidget_Entry_Value::CloseDropdownMenu(bool bIsFocusThis)
{
	if (!IsValid(MenuAnchor_Option))return;

	if (MenuAnchor_Option->IsOpen())
	{
		MenuAnchor_Option->Close();

        if (bIsFocusThis)
        {
            if (APlayerController* PC = GetOwningPlayer())
            {
                SetUserFocus(PC);
            }
        }
	}
}