// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCNListData_Base.generated.h"

#define MAKE_SET_GET(Type, Value)\
void Set##Value(const Type& In##Value) { Value = In##Value; }\
Type Get##Value()const { return Value; }

class UTexture2D;

//设置当前缓存值
DECLARE_DELEGATE_OneParam(FOnWriteSettingValue, const FText&);
//获取当前缓存值
DECLARE_DELEGATE_RetVal(FText, FOnReadSettingValue);

UCLASS(Transient)
class YCNUIDEMO_API UYCNListData_Base : public UObject
{
	GENERATED_BODY()

private:
	//数据ID
	UPROPERTY(Transient)
	FName DataID;

	//数据显示文本
	UPROPERTY(Transient)
	FText DataDisplayText;

	//数据描述
	UPROPERTY(Transient)
	FText DataDescription;

	//数据父类
	UPROPERTY(Transient)
	TObjectPtr<UYCNListData_Base>DataParent;

	//数据图标
	UPROPERTY(Transient)
	TObjectPtr<UTexture2D>DataIcon;

	//写
	FOnWriteSettingValue OnWriteSettingValue;
	//读
	FOnReadSettingValue OnReadSettingValue;

public:
	//设置/获取器
	MAKE_SET_GET(FName, DataID);
	MAKE_SET_GET(FText, DataDisplayText);
	MAKE_SET_GET(FText, DataDescription);
	MAKE_SET_GET(TObjectPtr<UYCNListData_Base>, DataParent);
	MAKE_SET_GET(TObjectPtr<UTexture2D>, DataIcon);

public:
	// Entry 通过这两个函数访问设置。
	bool TryReadSettingText(FText& OutValue) const;
	bool TryWriteSettingText(const FText& InValue) const;

	//检查写代理是否绑定了函数
	bool HasWriteBinding()const;

	//在注册数据时设置绑定。
	void SetSettingTextBinding(FOnReadSettingValue InReadDelegate, FOnWriteSettingValue InWriteDelegate);
};
