// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "DamageTypePair.h"
#include "DamageProfile.generated.h"



/**
 * 
 */
UCLASS(Blueprintable)
class FPSPROTOSANDBOX_API UDamageProfile : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly)
	TArray<UDamageTypePair*> DamageProfiles;
};
