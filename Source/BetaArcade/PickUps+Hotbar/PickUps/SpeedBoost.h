// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUps+Hotbar/PickUpBase.h"
#include "SpeedBoost.generated.h"

/**
 * 
 */
UCLASS()
class BETAARCADE_API ASpeedBoost : public APickUpBase
{
	GENERATED_BODY()


public:
	
	ASpeedBoost();

	//virtual void DuringTimer(class ABetaArcadeCharacter* Character)override;
	virtual void UseFromHotbar(class ABetaArcadeCharacter* Character) override;
	
};
