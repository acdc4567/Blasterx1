// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay){
    if (DisplayText) {
        DisplayText->SetText(FText::FromString(TextToDisplay));
    }
}


void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel,UWorld* InWorld){
    RemoveFromParent();
    Super::OnLevelRemovedFromWorld(InLevel,InWorld);
}