// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <queue>
#include "GameFramework/Actor.h"
#include "ISMController.generated.h"

UCLASS()
class UE4MINECS_API AISMController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AISMController();
public:
	//Component fields
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent = nullptr;

public:
	//ISM API functions
	void Initialize(UStaticMesh* InMesh) const;
	int32 GetInstanceCount() const;
	void CreateOrExpandTransformArray();
	int32 AddInstance(FTransform transform);
	int32 AddInstance();
	void SetTransform(int32 instanceIndex, const FTransform& transform);
	void RemoveInstance(int32 instanceIndex);
	void BatchUpdateTransform() const;
	void UpdateInstanceTransform(int32 instanceIndex, FTransform& transform);
	bool CheckIndexPool();
private:
	TArray<FTransform> transforms;
	TQueue<int> indexPool;
};
