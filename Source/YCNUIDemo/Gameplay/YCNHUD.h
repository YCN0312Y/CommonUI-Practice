// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "YCNHUD.generated.h"

UCLASS()
class YCNUIDEMO_API AYCNHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay()override;

private:
	UPROPERTY(EditAnywhere, Category = "YCN Widget")
	bool bIsVisibleBackground;
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "YCN|UI")
	void OnMainStackReady();
};
