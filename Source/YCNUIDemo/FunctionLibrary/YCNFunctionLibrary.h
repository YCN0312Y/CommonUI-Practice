// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "YCNFunctionLibrary.generated.h"

class UYCNWidget_ActivatableWidgetBase;
class UYCNWidget_MainStack;
class UYCNListData_Tab;
class UTexture2D;

UCLASS()
class YCNUIDEMO_API UYCNFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//根据标签获取软引用控件
	UFUNCTION(BlueprintPure, Category = "YCNFunctionLibrary")
	static TSoftClassPtr<UYCNWidget_ActivatableWidgetBase>GetSoftWidgetByTag(FGameplayTag InWidgetTag);
	//获取主堆栈控件
	UFUNCTION(BlueprintPure, Category = "YCNFunctionLibrary")
	static TSoftClassPtr<UYCNWidget_MainStack>GetMainStackWidget();
	//根据ID在数据资产中获取对应图标
	UFUNCTION(BlueprintPure, Category = "YCNFunctionLibrary")
	static UTexture2D* GetAssetImageById(const FName& InDataID);
};
