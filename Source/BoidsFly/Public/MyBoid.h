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
	FVector GetCurVelocity();
	bool GetIsCollosion();
	/*Value*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	int BirdId = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SpeedMin = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SpeedMax = 0.7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float ViewRadius = 100.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AovRadius = 20.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float CenterWeight = 0.05;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float FlowWeight = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AovWeight = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float FreeWeight = 0.9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float CollosionWeight = 0.02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float leaveWallWeight = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float TestBoxSize = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float LeaveTime = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FVector GoalDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool FindOther = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool UseGPU = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FVector Aov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FVector AovOut;
private:
	/*Value*/
	FVector CurVelocity;
	bool IsCollision = false;
	TArray<FVector> RaysVectors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesBird;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesWall;
	TArray<AActor*> IgnoryActors;
	FTimerHandle CollisionTimer;

	bool GetRaysVectors();
	void SetIsCollosionFalse();
	FVector ClampPos(FVector Pos);
};
