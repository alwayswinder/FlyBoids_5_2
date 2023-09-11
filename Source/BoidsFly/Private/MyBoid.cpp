// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBoid.h"
#include "Kismet\KismetSystemLibrary.h"
#include "../BoidsFly.h"
#include "../BoidsFlyGameModeBase.h"
#include "Kismet\GameplayStatics.h"


// Sets default values
AMyBoid::AMyBoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObjectTypesBird.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	ObjectTypesWall.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
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
	ABoidsFlyGameModeBase* Gm = Cast<ABoidsFlyGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (Gm && !(Gm->UseGPU))
	{
		UpdateBird(false);
	}
}

FVector AMyBoid::GetCurVelocity()
{
	return CurVelocity;
}

bool AMyBoid::GetIsCollosion()
{
	return IsCollision;
}

void AMyBoid::UpdateBird(bool UseComputeShader)
{
	CurAcceleration = FVector(0, 0, 0);

	//�ۺϣ�ͬ�У�����
	if (FindOther)
	{
		if (!UseComputeShader)
		{
			TArray<AMyBoid*> NearBoids;

			TArray<FHitResult> Hits;
			UKismetSystemLibrary::SphereTraceMultiForObjects(this, GetActorLocation(), GetActorLocation(), ViewRadius,
				ObjectTypesBird, false, IgnoryActors, EDrawDebugTrace::None, Hits, true);

			for (FHitResult hit : Hits)
			{
				if (hit.bBlockingHit)
				{
					AMyBoid* Bird = Cast<AMyBoid>(hit.GetActor());
					if (Bird && !Bird->GetIsCollosion())
					{
						NearBoids.Add(Bird);
					}
				}
			}
			if (NearBoids.Num() > 0 && !IsCollision)
			{
				FVector Center = FVector(0, 0, 0);
				Aov = Aov * FreeWeight;
				FVector Flow = FVector(0, 0, 0);
				int BoidNum = 0;

				for (AMyBoid* Bird : NearBoids)
				{
					FVector OffsetVector = Bird->GetActorLocation() - GetActorLocation();
					float Distence = OffsetVector.Size();

					if (Distence <= ViewRadius)
					{
						Center += Bird->GetActorLocation();
						Flow += Bird->GetCurVelocity();
						BoidNum++;

						if (Distence <= AovRadius)
						{
							Aov -= OffsetVector / (Distence * Distence);
						}
					}
				}
				if (BoidNum > 0)
				{
					CurAcceleration += (Center / BoidNum - GetActorLocation()) * CenterWeight;
					DebugVector = Center / BoidNum;
					CurAcceleration += (Flow + GoalDirection) / (float)BoidNum * FlowWeight;
				}
				CurAcceleration += Aov * AovWeight;
				//CurAcceleration = CurAcceleration.GetSafeNormal(0.0001f) * FMath::Clamp(CurAcceleration.Size(), 0.0f, 1.0f);
				//UE_LOG(LogTemp, Warning, TEXT("NearBoids"));
			}
		}
		else
		{
			int BoidNearNum = FMyBoidModule::Get().BoidInfoSave.BoidBase[BirdId].BoidNearNum;
			FVector Center = FMyBoidModule::Get().BoidInfoSave.BoidBase[BirdId].Center;
			FVector Flow = FMyBoidModule::Get().BoidInfoSave.BoidBase[BirdId].Flow;
			FVector AovOut = FMyBoidModule::Get().BoidInfoSave.BoidBase[BirdId].AovOut;
			if (BoidNearNum > 0 && !IsCollision)
			{
				Aov = Aov * FreeWeight - AovOut;
				CurAcceleration += (Center / BoidNearNum - GetActorLocation()) * CenterWeight;
				DebugVector = Center / BoidNearNum;
				CurAcceleration += (Flow + GoalDirection) / (float)BoidNearNum * FlowWeight;
				CurAcceleration += Aov * AovWeight;
			}
		}
	}

	if (GetRaysVectors())
	{
		for (FVector RayVector : RaysVectors)
		{
			FHitResult Hit;
			FVector End = GetActorLocation() + RayVector * ViewRadius;
			UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetActorLocation(), End, 5.0f,
				ObjectTypesWall, false, IgnoryActors, EDrawDebugTrace::None, Hit, true, FLinearColor::Green, FLinearColor::Red, 0.2f);
			if (!Hit.bBlockingHit)
			{
				CurAcceleration += RayVector * CollosionWeight;
				IsCollision = true;
				GetWorldTimerManager().SetTimer(CollisionTimer, this, &AMyBoid::SetIsCollosionFalse, LeaveTime, false, LeaveTime);
				//UE_LOG(LogTemp, Warning, TEXT("MeetCollosion"));
				break;
			}
		}
	}

	if (IsCollision)
	{
		FHitResult Hit;
		UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetActorLocation(), GetActorLocation(), AovRadius,
			ObjectTypesWall, false, IgnoryActors, EDrawDebugTrace::None, Hit, true);
		if (Hit.bBlockingHit)
		{
			FVector OffSetCollosion = GetActorLocation() - Hit.ImpactPoint;
			CurAcceleration += OffSetCollosion * leaveWallWeight / (OffSetCollosion.Size() * OffSetCollosion.Size());
		}
		//UE_LOG(LogTemp, Warning, TEXT("leaveWall"));
	}

	CurAcceleration = CurAcceleration.GetSafeNormal(0.0001f) * FMath::Clamp(CurAcceleration.Size(), 0.0f, 1.0f);

	CurVelocity += CurAcceleration;
	CurVelocity = CurVelocity.GetSafeNormal(0.0001f) * FMath::Clamp(CurVelocity.Size(), SpeedMin, SpeedMax);
	FVector NewLoc = GetActorLocation() + CurVelocity;
	NewLoc = ClampPos(NewLoc);
	SetActorLocation(NewLoc, true);
	SetActorRotation(FRotationMatrix::MakeFromX(CurVelocity.GetSafeNormal(0.0001f)).Rotator());
}

void AMyBoid::AddSelfToManage()
{
	FMyBoidModule::Get().BoidInfoSave.BoidBase.Add(BirdId, FMyBoidBase(GetActorLocation(), CurVelocity));
}

bool AMyBoid::GetRaysVectors()
{
	FHitResult Hit;
	FVector End = GetActorLocation() + GetActorForwardVector() * ViewRadius;
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, GetActorLocation(), End, 5.0f,
		ObjectTypesWall, false, IgnoryActors, EDrawDebugTrace::None, Hit, true);
	if (Hit.bBlockingHit)
	{
		RaysVectors.Empty();
		int RaysNum = 50;
		for (int i=1; i<RaysNum; i++)
		{
			RaysVectors.Add((GetActorRightVector() - GetActorForwardVector()) * (float(i) / float(RaysNum)) + GetActorForwardVector());
			RaysVectors.Add((GetActorRightVector() * -1.0f - GetActorForwardVector()) * (float(i) / float(RaysNum)) + GetActorForwardVector());
			RaysVectors.Add((GetActorUpVector() - GetActorForwardVector()) * (float(i) / float(RaysNum)) + GetActorForwardVector());
			RaysVectors.Add((GetActorUpVector() * -1.0f - GetActorForwardVector()) * (float(i) / float(RaysNum)) + GetActorForwardVector());
		}
		return true;
	}
	return false;
}

void AMyBoid::SetIsCollosionFalse()
{
	IsCollision = false;
}

FVector AMyBoid::ClampPos(FVector Pos)
{
	if (FMath::Abs(SpawnLocation.X - Pos.X) > TestBoxSize ||
		FMath::Abs(SpawnLocation.Y - Pos.Y) > TestBoxSize ||
		FMath::Abs(SpawnLocation.Z - Pos.Z) > TestBoxSize)
	{
		return SpawnLocation;
	}
	return Pos;
}
