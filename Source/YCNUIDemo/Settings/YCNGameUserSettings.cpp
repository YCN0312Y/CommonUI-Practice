// Fill out your copyright notice in the Description page of Project Settings.

#include "YCNUIDemo/Settings/YCNGameUserSettings.h"

UYCNGameUserSettings::UYCNGameUserSettings()
{

}

UYCNGameUserSettings* UYCNGameUserSettings::Get()
{
    return GEngine ? Cast<UYCNGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void UYCNGameUserSettings::SetCachedGameDifficultyText(const FText& InGameDifficultyText)
{
    Cached_GameDifficultyText = InGameDifficultyText;
}

const FText& UYCNGameUserSettings::GetCachedGameDifficultyText() const
{
    return Cached_GameDifficultyText.IsEmpty() ? FText::GetEmpty() : Cached_GameDifficultyText;
}
