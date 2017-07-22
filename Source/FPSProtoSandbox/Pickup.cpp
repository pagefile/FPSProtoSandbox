// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProtoSandbox.h"
#include "Pickup.h"
#include "Gun.h"


// Sets default values
APickup::APickup()
{
    // TODO - Start thinking about whether some of this code should go into InitializeComponents()
    // or PostInitializeComponents()

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBounds"));
    RootComponent = CollisionBounds;

    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupMesh"));
    Mesh->AttachTo(RootComponent);
    GunInstance = nullptr;
    PreviousOwner = nullptr;
}

void APickup::BeginPlay()
{
    //OnActorHit.AddDynamic(this, &APickup::OnHit);
}

void APickup::OnHit(AActor* self, AActor* other, FVector normal, const FHitResult &hitResult)
{

}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
    if(PreviousOwner != nullptr)
    {
        PrevOwnerTimeout -= DeltaTime;
        if(PrevOwnerTimeout <= 0.0f)
        {
            //CollisionBounds->IgnoreActorWhenMoving(PreviousOwner, false);
            PrevOwnerTimeout = 0.0f;
            PreviousOwner = nullptr;
        }
    }
}

AGun* APickup::GetGun(void)
{
    if(GunInstance == nullptr)
    {
        GunInstance = Cast<AGun>(GetWorld()->SpawnActor<AGun>(Weapon));
    }
    return GunInstance;
}

bool APickup::CanPickup(APawn* pawn)
{
    return !(pawn == PreviousOwner);
}

void APickup::SetPreviousOwner(APawn *owner, float time)
{
    time = fmax(1.0f, time);
    PreviousOwner = owner;
    PrevOwnerTimeout = time;
}