// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCNListData_Base.generated.h"

#define MAKE_SET_GET(Type, Value)\
void Set##Value(const Type& In##Value) { Value = In##Value; }\
Type Get##Value()const { return Value; }

class UTexture2D;

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_Base : public UObject
{
	GENERATED_BODY()

public:
	//初始化数据
	virtual void InitializationData();

private:
	//数据ID
	UPROPERTY()
	FName DataID;
	//数据显示文本
	UPROPERTY()
	FText DataDisplayText;
	//数据描述
	UPROPERTY()
	FText DataDescription;
	//数据父类
	UPROPERTY()
	TObjectPtr<UYCNListData_Base>DataParent;
	//数据图标
	UPROPERTY()
	TObjectPtr<UTexture2D>DataIcon;

public:
	MAKE_SET_GET(FName, DataID);
	MAKE_SET_GET(FText, DataDisplayText);
	MAKE_SET_GET(FText, DataDescription);
	MAKE_SET_GET(TObjectPtr<UYCNListData_Base>, DataParent);
	MAKE_SET_GET(TObjectPtr<UTexture2D>, DataIcon);
};
