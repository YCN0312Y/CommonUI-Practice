// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_Entry_Value.h"
#include "YCNUIDemo/UI/Widgets/Options/YCNWidget_DropdownOption.h"
#include "YCNUIDemo/Subsystems/YCNWidgetManagerSubsystem.h"
#include "YCNUIDemo/FunctionLibrary/YCNFunctionLibrary.h"
#include "YCNUIDemo/UI/Components/YCNWidget_ButtonBase.h"
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
    }
}

void UYCNWidget_Entry_Value::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	OwningValueData = Cast<UYCNListData_Value>(ListItemObject);
}

FReply UYCNWidget_Entry_Value::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
    if (CommonText_SelectedText)
    {
        CommonText_SelectedText->SetStyle(TextStyle_Selected);
    }
    if (CommonButton_DropdownButton)
    {
        CommonButton_DropdownButton->SetButtonIconColor(CommonButton_DropdownButton->IncoSelectedColor);
    }
    return FReply::Handled();
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

void UYCNWidget_Entry_Value::OnClicked_DropdownButton()
{
    if (!MenuAnchor_Option)
    {
        return;
    }

    if (MenuAnchor_Option->IsOpen())
    {
        MenuAnchor_Option->Close();
    }
    else
    {
        MenuAnchor_Option->Open(true);
    }
}

UUserWidget* UYCNWidget_Entry_Value::CreateOptionMenuContent()
{
    if (!IsValid(MenuAnchor_Option) || !IsValid(OwningValueData))
    {
        return nullptr;
    }

    UClass* WidgetClass = MenuAnchor_Option->MenuClass.Get();

    if (!WidgetClass
        || !WidgetClass->IsChildOf(UYCNWidget_DropdownOption::StaticClass()))
    {
        return nullptr;
    }

    UYCNWidget_DropdownOption* MenuWidget = CreateWidget<UYCNWidget_DropdownOption>(GetOwningPlayer(), WidgetClass);

    if (MenuWidget)
    {
        MenuWidget->OnUpdateSelectedText.BindUObject(this, &UYCNWidget_Entry_Value::OnDropdownOptionSelected);
        MenuWidget->SetOptionListData(OwningValueData);
    }

    // 返回的这个实例，才是 MenuAnchor 随后显示的菜单。
    return MenuWidget;
}

void UYCNWidget_Entry_Value::OnDropdownOptionSelected(const FText& InOptionText)
{
    if (IsValid(CommonText_SelectedText))
    {
        CommonText_SelectedText->SetText(InOptionText);
    }
}
