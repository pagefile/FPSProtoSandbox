// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProtoSandbox.h"
#include "Gun.h"
#include "FPSProtoSandboxCharacter.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("root"));
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeapMesh1p"));
    Mesh1P->AttachTo(RootComponent);
    FName lol;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
    IsTriggerDown = false;
    ReadyToFire = true;
    CurrentCycleTime = 0;
}

// Called every frame
void AGun::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
    CurrentCycleTime += DeltaTime;
    if(CurrentCycleTime >= CycleRate)
    {
        ReadyToFire = true;
    }
    if(IsAutomatic && IsTriggerDown && ReadyToFire)
    {
        OnFire();
    }
}

void AGun::EquipTo(AFPSProtoSandboxCharacter *pawn)
{
    MyPawn = pawn;
    Mesh1P->AttachTo(pawn->GetFirstPersonCameraComponent());
    Mesh1P->SetRelativeLocation(GunOffset);
    Mesh1P->SetVisibility(true);
    SetActorHiddenInGame(false);
    SetActorTickEnabled(true);
}

void AGun::Unequip()
{
    Mesh1P->AttachTo(nullptr);
    Mesh1P->SetVisibility(false);
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

FVector AGun::GetMuzzlePoint()
{
    const USkeletalMeshSocket *muzzleSocket = Mesh1P->GetSocketByName("MuzzlePoint");
    if(muzzleSocket)
    {
        FTransform muzzleTrans = muzzleSocket->GetSocketTransform(Mesh1P);
        FVector unused;
        return muzzleTrans.TransformPosition(unused);
    }
    return FVector(0.0f, 0.0f, 0.0f);
}

FVector AGun::GetMuzzleDir()
{
    return RootComponent->GetForwardVector();
}

FTransform AGun::GetMuzzleTransform()
{
    const USkeletalMeshSocket *muzzleSocket = Mesh1P->GetSocketByName("MuzzlePoint");
    if(muzzleSocket != nullptr)
    {
        FTransform muzzleTrans = muzzleSocket->GetSocketTransform(Mesh1P);
        muzzleTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
        return muzzleTrans;
    }
    return FTransform();
}

void AGun::OnFire()
{
    //FVector muzzlePoint = GetMuzzlePoint();
    //FVector muzzleDir = GetMuzzleDir();
    FTransform bulletTrans = GetMuzzleTransform();
    
    if (FireSound != nullptr)
    {
    	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    FActorSpawnParameters spawnParams;
    spawnParams.Instigator = MyPawn;
    
    AWeapProjectile *spawned = Cast<AWeapProjectile>(GetWorld()->SpawnActor((UClass *)FireProjectile, &bulletTrans, spawnParams));

    ReadyToFire = false;
    CurrentCycleTime = 0.0f;
}

void AGun::TriggerDown()
{
    IsTriggerDown = true;
    if(ReadyToFire)
    {
        OnFire();
    }
}

void AGun::TriggerUp()
{
    IsTriggerDown = false;
}