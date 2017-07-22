// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProtoSandbox.h"
#include "WeapProjectile.h"
#include "Explosion.h"


// Sets default values
AWeapProjectile::AWeapProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    DirectDamage = 5.0f;
}

// Called when the game starts or when spawned
void AWeapProjectile::BeginPlay()
{
	Super::BeginPlay();
    bExploded = false;
}

// Called every frame
void AWeapProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}