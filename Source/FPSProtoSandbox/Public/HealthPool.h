// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "DamageProfile.h"
#include "HealthPool.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROTOSANDBOX_API UHealthPool : public UObject
{
	GENERATED_BODY()

public:
    // Properties
    UPROPERTY(EditDefaultsOnly, Category=Gameplay)
    float MaxHealth;

    UPROPERTY(EditDefaultsOnly, Category=Gameplay)
    float Priority;     // Higher priority takes damage first. 0 Is base health
	
    UPROPERTY(EditDefaultsOnly, Category=Gameplay)
    UDamageProfile* DamageProfile;

protected:
    float CurrentHealth;
};
