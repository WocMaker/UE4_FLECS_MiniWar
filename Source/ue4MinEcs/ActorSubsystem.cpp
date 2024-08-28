// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSubsystem.h"
void UActorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}
void UActorSubsystem::InitActorSystem(UStaticMesh* InMeshEnermy, UStaticMesh* InMeshFriend, UStaticMesh* InMeshBeamRed, UStaticMesh* InMeshBeamBlue) 
{
	mInMeshEnermy = InMeshEnermy;
	mInMeshFriend = InMeshFriend;
	mInMeshBeam = InMeshBeamRed;
	mInMeshBeamBlue = InMeshBeamBlue;
}
void UActorSubsystem::SpawnUnitEntity(FVector location, FRotator rotation, bool isFriend, int spawnNum) {

	for (int spawnCnt = 0; spawnCnt <= spawnNum; spawnCnt++) {
		FVector locationRand = location + FVector(1000 * FMath::SRand() - 500, 0, 1000 * FMath::SRand() - 500);
		AFriendActor* FriendUnit = GetWorld()->SpawnActor<AFriendActor>(AFriendActor::StaticClass(), locationRand, FRotator::ZeroRotator);
		ABeamActor* BeamUnit = GetWorld()->SpawnActor<ABeamActor>(ABeamActor::StaticClass(), locationRand, FRotator::ZeroRotator);
		
		if (isFriend) {
			FriendUnit->SetStaticMesh(mInMeshFriend);
			BeamUnit->SetStaticMesh(mInMeshBeam);
		}
			
		else {
			FriendUnit->SetStaticMesh(mInMeshEnermy);
			BeamUnit->SetStaticMesh(mInMeshBeamBlue);
		}
		

		FriendUnit->SetFriend(isFriend);

		FActorPair NewPair;
		NewPair.UnitActor = FriendUnit;
		NewPair.BeamActor = BeamUnit;
		ActorPairs.Add(NewPair);
	}
}
bool CheckInRange(FVector rayStart, FVector rayEnd)
{
	FVector rayVec = rayEnd - rayStart;
	float distance = FVector::Distance(rayStart, rayEnd);
	if (distance < 2000)
		return true;
	else
		return false;

}
void UActorSubsystem::UpdateBeam(float DeltaTime)
{
	for (int32 i = ActorPairs.Num() - 1; i >= 0; --i)
	{
		auto UnitActor = ActorPairs[i].UnitActor;
		auto BeamActor = ActorPairs[i].BeamActor;
		FVector rayStart = FVector::ZeroVector;
		FVector rayEnd = UnitActor->GetActorLocation();
		if (UnitActor->GetTeam()) {
			rayStart = FVector(0, -2000, 600);
		}
		else {
			rayStart = FVector(0, 2000, 600);
		}
		FVector rayVec = rayEnd - rayStart;
		float distance = FVector::Distance(rayStart, rayEnd);

		if ((distance < 2000) && UnitActor->Health >= 0) {
			BeamActor->TragetTransform = FTransform(rayVec.GetSafeNormal().ToOrientationRotator(), rayStart, FVector(0.005 * distance, 0.01, 0.01));
			UnitActor->Health -= 50 * DeltaTime;
			BeamActor->OutRange = false;
		}
		else {
			BeamActor->OutRange = true;
		}
		if (UnitActor->Health < 0)
		{
			/*  
			// 优化版本 删除Actor
			if (ActorPairs[i].UnitActor)
			{
				ActorPairs[i].UnitActor->Destroy();
			}
			if (ActorPairs[i].BeamActor)
			{
				ActorPairs[i].BeamActor->Destroy();
			}
			*/
			// 一致处理
			BeamActor->OutRange = true;
			ActorPairs.RemoveAt(i);
		}
	}
}