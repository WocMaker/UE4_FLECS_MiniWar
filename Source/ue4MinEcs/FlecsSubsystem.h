// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "flecs.h"
#include "ISMController.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Math/UnrealMathUtility.h" 
#include "FlecsSubsystem.generated.h"


struct FlecsISMIndex
{
	int Value;
};

struct FlecsISMBeamIndex
{
	int Value;
};
struct FlecsIsmRef
{
	AISMController* Value;
};

struct FlecsProjectIsmRef
{
	AISMController* Value;
};
struct FlecsBeamCd
{
	int BeamCdTimer;
};
struct FlecsTarget
{
	flecs::entity Target;
	float distance;
};
struct FlecsBeamTransform
{
	FTransform Value;
};

struct FlecsUnit
{
	FTransform Pos;
	bool IsFriend;
	float Vx;
	float Vy;
	float Vz;
	float SteerTimer;
	bool EnermySpotted;
};
struct FlecsHealth
{
	float Health;
};
struct Enermy {};
struct Units {};
struct Beam {};
struct Friend {};

USTRUCT(BlueprintType)
struct FFlecsEntityHandle
{
	GENERATED_USTRUCT_BODY()
		FFlecsEntityHandle() {}
	FFlecsEntityHandle(int inId)
	{
		FlecsEntityId = inId;
	}
	UPROPERTY(BlueprintReadWrite)
		int FlecsEntityId;
};

/**
 * 
 */
UCLASS()
class UE4MINECS_API UFlecsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	flecs::world* GetEcsWorld() const;
	
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* EnermyRenderer = nullptr;

	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* FriendRenderer = nullptr;
	
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* BeamRenderer = nullptr;

	AISMController* EnermyISM = nullptr;
	AISMController* FriendISM = nullptr;
	AISMController* BeamRedISM = nullptr;
	AISMController* BeamBlueISM = nullptr;

	UFUNCTION(BlueprintCallable, Category = "FLECS")
	void InitFlecs(UStaticMesh* InMeshEnermy, UStaticMesh* InMeshFriend, UStaticMesh* InMeshBeamRed, UStaticMesh* InMeshBeamBlue);

	UFUNCTION(BlueprintCallable, Category = "FLECS")
	void SpawnUnitEntity(FVector location, FRotator rotation, bool isFriend, int spawnNum);
protected:
	FTickerDelegate OnTickDelegate;
	FDelegateHandle OnTickHandle;
	flecs::world* ECSWorld = nullptr;
	UInstancedStaticMeshComponent* vIsmMap[3] = {nullptr, nullptr, nullptr};
private:
	bool Tick(float DeltaTime);
	void InitIsmRenderers(UStaticMesh* InMeshEnermy, UStaticMesh* InMeshFriend, UStaticMesh* InMeshBeamRed, UStaticMesh* InMeshBeamBlue);

};