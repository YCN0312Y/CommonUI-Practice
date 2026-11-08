// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "YCNGameInstance.generated.h"

UCLASS()
class YCNUIDEMO_API UYCNGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	void SetupMapLoadingScreen(const FString& MapName);
	
};
