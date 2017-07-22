// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Explosion.h"
#include "WeapProjectile.generated.h"

UCLASS()
class FPSPROTOSANDBOX_API AWeapProjectile : public AActor
{
	GENERATED_BODY()

public:
    // UPropterties
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    float DirectDamage;

    UPROPERTY(EditDefaultsOnly, Category = Effects)
    TSubclassOf<AExplosion> Explosion;

protected:
    bool bExploded;

public:	
	// Sets default values for this actor's properties
	AWeapProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
