// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProtoSandbox.h"
#include "DamageTypePair.h"
#include "DamageProfile.h"


UDamageTypePair::UDamageTypePair()
{
    DamageType = EDamageType::Default;
    DamageScale = 1.0f;
}