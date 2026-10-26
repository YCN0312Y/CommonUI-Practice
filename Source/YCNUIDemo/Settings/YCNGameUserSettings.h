// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "YCNGameUserSettings.generated.h"

#define MAKE_SET_GET(Type, Value)\
	void Set##Value(const Type& In##Value)\
	{\
		Value = In##Value;\
	}\
	Type Get##Value()const \
	{ \
		return Value; \
	}

UCLASS()
class YCNUIDEMO_API UYCNGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UYCNGameUserSettings();

	static UYCNGameUserSettings* Get();
private:
	//游戏难度文本
	UPROPERTY(Config)
	FText CachedGameDifficultyText;
	
public:
	MAKE_SET_GET(FText, CachedGameDifficultyText);
};
