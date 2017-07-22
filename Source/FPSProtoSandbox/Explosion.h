// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class FPSPROTOSANDBOX_API AExplosion : public AActor
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = Effects)
    UParticleSystemComponent *ExplosionParticle;

    UPROPERTY(EditDefaultsOnly, Category = Effects)
    USoundBase *ExplosionSound;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    float ExplosionKnockback;

    FHitResult SurfaceHit;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    float Damage;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    float Falloff;

public:	
	// Sets default values for this actor's properties
	AExplosion();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
