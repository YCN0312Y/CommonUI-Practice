#pragma once

#include "CoreMinimal.h"

DEFINE_LOG_CATEGORY_STATIC(LogYCNCommonUI, Display, All);

namespace YCNDebug
{
	inline void Print(const FString& InMessage, float InDisplayTime = 5.f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, InDisplayTime, FColor::MakeRandomColor(), FString::Printf(TEXT("%s"), *InMessage));
		}
		UE_LOG(LogYCNCommonUI, Warning, TEXT("%s"), *InMessage);
	}
}