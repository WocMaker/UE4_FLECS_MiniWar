// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FriendActor.h"
#include "BeamActor.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ISMController.h"
#include "ActorSubsystem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FActorPair
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Actor Pair")
	AFriendActor* UnitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Actor Pair")
	ABeamActor* BeamActor;
};

UCLASS()
class UE4MINECS_API UActorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:	
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	UFUNCTION(BlueprintCallable, Category = "ActorCompare")
	void SpawnUnitEntity(FVector location, FRotator rotation, bool isFriend, int spawnNum);
	
	UFUNCTION(BlueprintCallable, Category = "ActorCompare")
	void UpdateBeam(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "ActorCompare")
	void InitActorSystem(UStaticMesh* InMeshEnermy, UStaticMesh* InMeshFriend, UStaticMesh* InMeshBeamRed, UStaticMesh* InMeshBeamBlue);
public:
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* EnermyRenderer = nullptr;

	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* FriendRenderer = nullptr;

	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* BeamRenderer = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMesh* mInMeshEnermy = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMesh* mInMeshFriend = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMesh* mInMeshBeam = nullptr;

	UPROPERTY(EditAnywhere)
	UStaticMesh* mInMeshBeamBlue = nullptr;
	

	TArray<FActorPair> ActorPairs;

private:
	TSubclassOf<AFriendActor> MyFriendClass;
};
