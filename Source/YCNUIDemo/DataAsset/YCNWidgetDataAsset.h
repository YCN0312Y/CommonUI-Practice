// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "YCNWidgetDataAsset.generated.h"

class UYCNListData_Base;
class UYCNWidget_EntryBase;
class UTexture2D;

UCLASS()
class YCNUIDEMO_API UYCNWidgetDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	//数据列表控件
	UPROPERTY(EditAnywhere, Category = "YCN Option Widget", meta = (ForceInlineRow))
	TMap<TSubclassOf<UYCNListData_Base>, TSoftClassPtr<UYCNWidget_EntryBase>>DataListWidget;
	//图标列表
	UPROPERTY(EditAnywhere, Category = "YCN Option Widget", meta = (ForceInlineRow))
	TMap<FName, TSoftObjectPtr<UTexture2D>>ImageList;

public:
	TSubclassOf<UYCNWidget_EntryBase>FindWidgetByListData(const UYCNListData_Base* InListData);

	TSoftObjectPtr<UTexture2D>FindImageByDataID(const FName& InDataID);
};
