#pragma once
#include "ObjectBase.h"

UENUM()
enum class EDamageType : uint8
{
    Default             UMETA(DisplayName = "Default"),
    Kinetic             UMETA(DisplayName = "Kinetic"),
    Energy              UMETA(DisplayName = "Energy"),
    Explosive           UMETA(DisplayName = "Explosive"),
    ArmorPiercing       UMETA(DisplayName = "ArmorPiercing"),
    Count,
};