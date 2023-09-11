// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidsManager.generated.h"

struct FMyBoidBase
{
	FMyBoidBase(FVector Pos, FVector Vel) 
	{
		Position = Pos;
		Velocity = Vel;
	}
	FVector Position = FVector(0, 0, 0);
	FVector Velocity = FVector(0, 0, 0);
	FVector Center = FVector(0, 0, 0);
	FVector Flow = FVector(0, 0, 0);
	FVector AovOut = FVector(0, 0, 0);
	int BoidNearNum = 0;
};

struct FMyBoidAttribute
{
	TMap<int, FMyBoidBase> BoidBase;
	float AovRadius = 20;
	float ViewRadius = 100;
};

UCLASS()
class BOIDSFLY_API ABoidsManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoidsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
	bool UseGPU = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
	FVector GoalDirection;
	
	UFUNCTION(BlueprintCallable)
	void ComputeBoid();
	UFUNCTION(BlueprintCallable)
	void ClearBoidBase();

	FCriticalSection RenderEveryFrameLock;
	
public:
	FMyBoidAttribute BoidInfoSave;
	
	void RunComputeShader(FRHICommandListImmediate& RHICmdList);
	void GetComputeShaderResult(FRHICommandListImmediate& RHICmdList);

private:
	FBufferRHIRef BoidBaseBuffer;
	FUnorderedAccessViewRHIRef BoidBaseRecordsUAV;
};
