// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/UI/Components/YCNWidget_ListViewBase.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "YCNUIDemo/DataAsset/YCNWidgetDataAsset.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"

TSubclassOf<UUserWidget> UYCNWidget_ListViewBase::GetDesiredEntryClassForItem(UObject* Item) const
{
    if (!IsDesignerPreview() && IsValid(WidgetDataAsset))
    {
        const UYCNListData_Base* Data = Cast<UYCNListData_Base>(Item);

        if (IsValid(Data))
        {
            const TSubclassOf<UYCNWidget_EntryBase> EntryClass = WidgetDataAsset->FindWidgetByListData(Data);

            if (EntryClass)
            {
                return EntryClass;
            }
        }
    }

    return Super::GetDesiredEntryClassForItem(Item);
}
