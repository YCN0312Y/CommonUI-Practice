// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "CommonTextBlock.h"

void UYCNWidget_EntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SetEnrtyDisplayText(CastChecked<UYCNListData_Base>(ListItemObject)->GetDataDisplayText());

}

void UYCNWidget_EntryBase::SetEnrtyDisplayText(const FText& InDisplayText)
{
	CommonText_DisplayText->SetText(InDisplayText);
}
