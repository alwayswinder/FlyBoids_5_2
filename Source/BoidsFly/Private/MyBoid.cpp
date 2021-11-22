// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBoid.h"
#include "Kismet\KismetSystemLibrary.h"
// Sets default values
AMyBoid::AMyBoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyBoid::BeginPlay()
{
	Super::BeginPlay();
	CurVelocity = GetActorForwardVector() * (SpeedMax + SpeedMin) / 2;
}

// Called every frame
void AMyBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurAcceleration = FVector(0,0,0);


	TArray<AMyBoid*> NearBoids;
	TArray<FHitResult> Hits;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	TArray<AActor*> IgnoryActors;
	UKismetSystemLibrary::SphereTraceMultiForObjects(this, GetActorLocation(), GetActorLocation(), ViewRadius,
		ObjectTypes, false, IgnoryActors, EDrawDebugTrace::None, Hits, true);
	for (FHitResult hit : Hits)
	{
		AMyBoid* Bird; = Cast<AMyBoid>(hit.Actor);
		if (Bird)
		{
			NearBoids.Add(Bird);
		}
	}
	if (NearBoids.Num() > 0)
	{
		FVector Center = FVector(0, 0, 0);
		FVector OffSet = FVector(0, 0, 0);
		FVector Direction = FVector(0, 0, 0);
		int BoidNum = 0;
		for (AMyBoid* Bird : NearBoids)
		{
			OffSet
		}

	}
}

