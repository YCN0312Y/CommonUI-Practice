// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "YCNUIDemo/UI/ListData/YCNListData_Base.h"
#include "YCNListData_Value.generated.h"

UCLASS(Abstract)
class YCNUIDEMO_API UYCNListData_Value : public UYCNListData_Base
{
	GENERATED_BODY()

protected:
	//是否拥有默认值
	UPROPERTY(Transient)
	bool bHasDefalutValue = false;

public:
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

		TOptional<TValueType>OwningDefaultValue;

	protected:
		//返回是否拥有默认值
		bool HasDefaultValue()const { return OwningDefaultValue.IsSet(); }

	public:
		virtual ~TYCNSettingBinding() = default;

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

		//是否可以重置为默认值
		virtual bool CanResetToDefault()const { return false; }

		virtual bool ResetToDefault()const
		{
			return CanResetToDefault() && TryWrite(GetOwningDefaultValue());
		}

		//设置默认值
		void SetOwningDefaultValue(const TValueType& InValue)
		{
			OwningDefaultValue = InValue;
		}

		//获取默认值
		const TValueType& GetOwningDefaultValue()const
		{
			check(OwningDefaultValue.IsSet());
			return OwningDefaultValue.GetValue();
		}
	};
	virtual bool CanResetToDefault() const { return false; }
	//重置为默认值
	virtual bool ResetToDefault()const { return false; }
};