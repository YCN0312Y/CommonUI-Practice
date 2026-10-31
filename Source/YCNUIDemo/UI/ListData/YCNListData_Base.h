// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YCNListData_Base.generated.h"

#define MAKE_SET_GET(Type, Value)\
void Set##Value(const Type& In##Value) { Value = In##Value; }\
Type Get##Value()const { return Value; }

class UTexture2D;

UCLASS(Abstract)
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

	UPROPERTY(Transient)
	TArray<UYCNListData_Base*>ChildDataList;

public:
	//设置/获取器
	MAKE_SET_GET(FName, DataID);
	MAKE_SET_GET(FText, DataDisplayText);
	MAKE_SET_GET(FText, DataDescription);
	MAKE_SET_GET(TObjectPtr<UYCNListData_Base>, DataParent);
	MAKE_SET_GET(TObjectPtr<UTexture2D>, DataIcon);

	//添加子类
	void AddChildData(UYCNListData_Base* InChildData);
	//根据子类数据ID获取数据
	UYCNListData_Base* GetDataByChildDataID(const FName& InDataID);
	//获取子类数据列表
	TArray<UYCNListData_Base*>GetChildDataList()const { return ChildDataList; }

	template<typename TValue>
	struct TYCNSettingBinding
	{
	public:
		using TValueType = TValue;
		//读取代理
		using FReadDelegate = TDelegate<bool(TValue&)>;
		//写入代理
		using FWriteDelegate = TDelegate<bool(const TValue&)>;

	private:
		FReadDelegate ReadDelegate;
		FWriteDelegate WriteDelegate;

	public:
		void Bind(FReadDelegate InReadDelegate, FWriteDelegate InWriteDelegate)
		{
			ReadDelegate = MoveTemp(InReadDelegate);
			WriteDelegate = MoveTemp(InWriteDelegate);
		}

		bool TryRead(TValue& OutValue) const
		{
			return ReadDelegate.IsBound() && ReadDelegate.Execute(OutValue);
		}

		bool TryWrite(const TValue& InValue) const
		{
			return WriteDelegate.IsBound() && WriteDelegate.Execute(InValue);
		}

		bool HasWriteBinding() const
		{
			return WriteDelegate.IsBound();
		}
	};

};
