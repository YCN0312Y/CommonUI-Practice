// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/DataAsset/YCNWidgetDataAsset.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNUIDemo/UI/Widgets/Options/EntryWidget/YCNWidget_EntryBase.h"
#include "Engine/AssetManager.h"

TSubclassOf<UYCNWidget_EntryBase> UYCNWidgetDataAsset::FindWidgetByListData(const UYCNListData_Base* InListData)
{
	if (!IsValid(InListData))return TSubclassOf<UYCNWidget_EntryBase>();

	for (UClass* DataClass = InListData->GetClass();
		DataClass && DataClass->IsChildOf(UYCNListData_Base::StaticClass());
		DataClass = DataClass->GetSuperClass())
	{
		const TSubclassOf<UYCNListData_Base>Key(DataClass);

		const TSoftClassPtr<UYCNWidget_EntryBase> EntryWidget = DataListWidget.FindRef(Key);

		UClass* LoadedClass = EntryWidget.LoadSynchronous();

		if (IsValid(LoadedClass) && !LoadedClass->HasAnyClassFlags(CLASS_Abstract))
		{
			return LoadedClass;
		}

		return nullptr;
	}
	return nullptr;
}
