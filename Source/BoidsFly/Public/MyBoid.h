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
	float SpeedMin = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SpeedMax = 2.0;
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
	float FreeWeight = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float CollosionWeight = 2.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float leaveWallWeight = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float TestBoxSize = 250;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float LeaveTime = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FVector GoalDirection;

private:
	/*Value*/
	FVector CurVelocity;
	FVector Aov;
	bool IsCollision = false;
	TArray<FVector> RaysVectors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesBird;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesWall;
	TArray<AActor*> IgnoryActors;
	FTimerHandle CollisionTimer;

	bool GetRaysVectors();
	void SetIsCollosionFalse();
	float ClampPos(float Pos);
};
