// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCNListData_Base.generated.h"

#define MAKE_SET_GET(Type, Value)\
void Set##Value(const Type& In##Value) { Value = In##Value; }\
Type Get##Value()const { return Value; }

UCLASS()
class YCNUIDEMO_API UYCNListData_Base : public UObject
{
	GENERATED_BODY()
	
private:
	//数据ID
	FName DataID;
	//数据显示文本
	FText DataDisplayText;
	//数据描述
	FText DataDescription;

public:
	MAKE_SET_GET(FName, DataID);
	MAKE_SET_GET(FText, DataDisplayText);
	MAKE_SET_GET(FText, DataDescription);
};
