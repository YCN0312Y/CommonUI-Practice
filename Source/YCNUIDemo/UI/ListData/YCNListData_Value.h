// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNListData_Value.generated.h"

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_Value : public UYCNListData_Base
{
	GENERATED_BODY()

protected:
	//是否可以重置为默认值
	virtual bool CanResetToDefault()const { return false; }

	//返回是否拥有默认值
	bool HasDefaultValue()const { return bHasDefalutValue; }

public:
	//重置为默认值
	virtual bool ResetToDefault()const { return false; }


protected:
	//是否拥有默认值
	UPROPERTY(Transient)
	bool bHasDefalutValue = false;
};