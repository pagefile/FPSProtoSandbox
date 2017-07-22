// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "DamageTypes.h"
#include "DamageTypePair.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROTOSANDBOX_API UDamageTypePair : public UObject
{
	GENERATED_BODY()
	
    public:
        EDamageType DamageType;
        float DamageScale;	
	    
        UDamageTypePair();
};
