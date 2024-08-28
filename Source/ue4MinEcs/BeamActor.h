// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeamActor.generated.h"

UCLASS()
class UE4MINECS_API ABeamActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeamActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SteerFunc(float DeltaTime);
	void MoveFunc(float DeltaTime);
	void BoundFunc(float DeltaTime);
	void HideDeadFunc(float DeltaTime);
	void ShowFunc();
	void SetFriend(bool bisFriend);
	void SetStaticMesh(UStaticMesh* InStaticMesh);
	void UpdateBeamStatus(bool bIsOutRange, FTransform TarTrans);
	bool GetTeam();
	float Health;
	float SteerTimer;
	float Vx;
	float Vy;
	float Vz;
	bool IsFriend;
	bool OutRange;
	FTransform TragetTransform;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MyStaticMesh;
};
