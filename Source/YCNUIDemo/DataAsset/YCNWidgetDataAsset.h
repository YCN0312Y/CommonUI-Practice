// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "YCNWidgetDataAsset.generated.h"

class UYCNListData_Base;
class UYCNWidget_EntryBase;

UCLASS()
class YCNUIDEMO_API UYCNWidgetDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "YCN Option Widget", meta = (ForceInlineRow))
	TMap<TSubclassOf<UYCNListData_Base>, TSoftClassPtr<UYCNWidget_EntryBase>>DataListWidget;

public:
	TSubclassOf<UYCNWidget_EntryBase>FindWidgetByListData(const UYCNListData_Base* InListData);
};
