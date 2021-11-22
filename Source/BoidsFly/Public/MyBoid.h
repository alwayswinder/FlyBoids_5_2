// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBoid.generated.h"

UCLASS()
class BOIDSFLY_API AMyBoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBoid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*Value*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SpeedMin = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SpeedMax = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float ViewRadius = 50.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AovRadius = 20.0;
private:
	/*Value*/
	FVector CurVelocity;
};
