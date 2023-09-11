// Copyright Epic Games, Inc. All Rights Reserved.


#include "BoidsFlyGameModeBase.h"
#include "BoidsFly.h"
#include "Kismet\GameplayStatics.h"
#include "MyBoid.h"


ABoidsFlyGameModeBase::ABoidsFlyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABoidsFlyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoidsFlyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

