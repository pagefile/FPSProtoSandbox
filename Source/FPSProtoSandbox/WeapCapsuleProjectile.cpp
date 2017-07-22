// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProtoSandbox.h"
#include "WeapCapsuleProjectile.h"


// Sets default values
AWeapCapsuleProjectile::AWeapCapsuleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // TODO: Maybe this should be a mesh component instead of static mesh?
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjMesh"));

    CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;
    CollisionComp->SetVisibility(false);

    RootComponent = CollisionComp;

    InitialLifeSpan = 3.0f;

    Mesh->AttachTo(RootComponent);
}

void AWeapCapsuleProjectile::PostActorCreated()
{
    CollisionComp->SetSimulatePhysics(true);
    //CollisionComp->IgnoreActorWhenMoving(Instigator, true);
    CollisionComp->IgnoreActorWhenMoving(Instigator, true);
}

// Called when the game starts or when spawned
void AWeapCapsuleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapCapsuleProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

