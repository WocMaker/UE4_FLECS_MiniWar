// Fill out your copyright notice in the Description page of Project Settings.


#include "FlecsSubsystem.h"


flecs::world* UFlecsSubsystem::GetEcsWorld() const { return ECSWorld; }

void Steer_System(flecs::iter& it, size_t i, FlecsUnit& fu)
{	
	float steerCD = 2;
	if (fu.SteerTimer > steerCD) {
		fu.Vx = FMath::RandRange(-100, 100);
		fu.Vy = fu.EnermySpotted ? FMath::RandRange(-100, 100) : (fu.IsFriend ? -200 : 200);
		fu.Vz = FMath::RandRange(-200, 200);
		fu.SteerTimer = 0;
	}
	else {
		fu.SteerTimer += it.delta_time();
	}
}

void Move_System(flecs::iter& it, size_t i, FlecsUnit& fu)
{
	FVector movementDirection = FVector(fu.Vx, fu.Vy, fu.Vz).GetSafeNormal();
	FVector newlocation = fu.Pos.GetLocation() + FVector(fu.Vx * it.delta_time(), fu.Vy * it.delta_time(), fu.Vz * it.delta_time());
	FRotator newRotation = movementDirection.ToOrientationRotator();
	fu.Pos = FTransform(newRotation, newlocation);
}
FVector LoopPosCal(FVector NewLocation, float maxPosX, float maxPosY, float maxPosZ)
{
	if (NewLocation.X > maxPosX)
	{
		NewLocation.X = -maxPosX;
	}
	else if (NewLocation.X < -maxPosX)
	{
		NewLocation.X = maxPosX;
	}

	if (NewLocation.Y > maxPosY)
	{
		NewLocation.Y = -maxPosY;
	}
	else if (NewLocation.Y < -maxPosY)
	{
		NewLocation.Y = maxPosY;
	}

	if (NewLocation.Z > maxPosZ)
	{
		NewLocation.Z = 2000;
	}
	else if (NewLocation.Z < 200)
	{
		NewLocation.Z = 2000;
	}

	return NewLocation;
}
void SpaceLoop_System(FlecsUnit& fu)
{
	float maxPosX = 3000;
	float maxPosY = 5000;
	float maxPosZ = 4000;
	FVector newPos = LoopPosCal(fu.Pos.GetLocation(), maxPosX, maxPosY, maxPosZ);
	fu.Pos = FTransform(FRotator::ZeroRotator, newPos);
}

void Beam_System(flecs::iter& it, size_t i, FlecsUnit& fu, FlecsHealth& fh, FlecsBeamTransform& fbt )
{
	FVector rayStart = FVector::ZeroVector;
	if (fu.IsFriend) {
		rayStart = FVector(0, -2000, 600);
	}
	else {
		rayStart = FVector(0, 2000, 600);
	}
	float cdMaxTime = 3;
	FVector rayEnd = fu.Pos.GetLocation();
	FVector rayVec = rayEnd - rayStart;
	float distance = FVector::Distance(rayStart, rayEnd);
	//float lengthScale = (fbcd.BeamCdTimer / cdMaxTime) * 0.005;
	if ((distance < 2000) && (fh.Health >= 0)) {
		fbt.Value = FTransform(rayVec.GetSafeNormal().ToOrientationRotator(), rayStart, FVector(0.005 * distance, 0.01, 0.01));
		fh.Health -= 50 * it.delta_time();
	}
	else {
		fbt.Value = FTransform(FRotator::ZeroRotator, rayStart, FVector(0.0001, 0.0001, 0.0001));
	}
}

void Render_System(FlecsUnit& fu, FlecsISMIndex& fi, FlecsIsmRef& fr, FlecsBeamTransform& fbt, FlecsISMBeamIndex& fbi, FlecsProjectIsmRef& fpr)
{
	fr.Value->UpdateInstanceTransform(fi.Value, fu.Pos);
	fpr.Value->UpdateInstanceTransform(fbi.Value, fbt.Value);
}

void ReleaseDead_System(flecs::iter& it, size_t i, FlecsISMIndex& fi, FlecsIsmRef& fr, FlecsISMBeamIndex& fbi, FlecsProjectIsmRef& fpr, FlecsHealth& fh)
{
	if (fh.Health > 0)
		return;
	fr.Value->RemoveInstance(fi.Value);
	fpr.Value->RemoveInstance(fbi.Value);
	it.entity(i).destruct();
}
void UFlecsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	OnTickDelegate = FTickerDelegate::CreateUObject(this, &UFlecsSubsystem::Tick);
	OnTickHandle = FTicker::GetCoreTicker().AddTicker(OnTickDelegate);

	//sets title in Flecs Explorer
	char name[] = { "Minimum Viable Flecs" };
	char* argv = name;
	ECSWorld = new flecs::world(1, &argv);

	//flecs explorer and monitor
	GetEcsWorld()->import<flecs::monitor>();
	GetEcsWorld()->set<flecs::Rest>({});

	//expose values with names to Flecs Explorer for easier inspection & debugging
	GetEcsWorld()->component<FlecsUnit>().member<FTransform>("Current transform");
	GetEcsWorld()->component<FlecsISMIndex>().member<int>("ISM Render index");

	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has started!"));
	Super::Initialize(Collection);
}

void UFlecsSubsystem::InitIsmRenderers(UStaticMesh* InMeshEnermy, UStaticMesh* InMeshFriend, UStaticMesh* InMeshBeamRed, UStaticMesh* InMeshBeamBlue)
{
	//Spawn an actor and add an Instanced Static Mesh component to it. This will render our entities.
	FActorSpawnParameters SpawnInfo;

	EnermyISM = Cast<AISMController>(GetWorld()->SpawnActor(AISMController::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnInfo));
	EnermyISM->Initialize(InMeshEnermy);

	FriendISM = Cast<AISMController>(GetWorld()->SpawnActor(AISMController::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnInfo));
	FriendISM->Initialize(InMeshFriend);

	BeamRedISM = Cast<AISMController>(GetWorld()->SpawnActor(AISMController::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnInfo));
	BeamRedISM->Initialize(InMeshBeamRed);

	BeamBlueISM = Cast<AISMController>(GetWorld()->SpawnActor(AISMController::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnInfo));
	BeamBlueISM->Initialize(InMeshBeamBlue);
}

void UFlecsSubsystem::InitFlecs(UStaticMesh* InMeshEnermy, UStaticMesh* InMeshFriend, UStaticMesh* InMeshBeamRed, UStaticMesh* InMeshBeamBlue)
{

	// int ism spawner
	InitIsmRenderers(InMeshEnermy, InMeshFriend, InMeshBeamRed, InMeshBeamBlue);

    // register systems
	auto system_Steer = GetEcsWorld()->system
		<FlecsUnit>
		("Speed System")
		.each(Steer_System);

	auto system_Move = GetEcsWorld()->system
		<FlecsUnit>
		("Move System")
		.each(Move_System);

	auto system_SpaceLoop = GetEcsWorld()->system
		<FlecsUnit>
		("Space Loop System")
		.each(SpaceLoop_System);

	auto system_Beam = GetEcsWorld()->system
		<FlecsUnit, FlecsHealth, FlecsBeamTransform>
		("Beam System")
		.each(Beam_System);

	auto system_Render = GetEcsWorld()->system
		<FlecsUnit, FlecsISMIndex, FlecsIsmRef, FlecsBeamTransform, FlecsISMBeamIndex, FlecsProjectIsmRef>
		("Render System")
		.each(Render_System);

	auto system_Reaper = GetEcsWorld()->system
		<FlecsISMIndex, FlecsIsmRef, FlecsISMBeamIndex, FlecsProjectIsmRef, FlecsHealth>
		("Release Dead System")
		.each(ReleaseDead_System);

	UE_LOG(LogTemp, Warning, TEXT("All System initialized!"));
}

void UFlecsSubsystem::Deinitialize()
{
	FTicker::GetCoreTicker().RemoveTicker(OnTickHandle);

	if (ECSWorld)
	{
		delete ECSWorld;
		ECSWorld = nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("UUnrealFlecsSubsystem has shut down!"));
	Super::Deinitialize();
}

void UFlecsSubsystem::SpawnUnitEntity(FVector location, FRotator rotation, bool isFriend, int spawnNum)
{
	for (int spawnCnt = 0; spawnCnt <= spawnNum; spawnCnt++) {
		FVector locationRand = location + FVector(1000 * FMath::SRand() - 500, 0, 1000 * FMath::SRand() - 500);
		
		if (isFriend) {
			auto IsmID = FriendISM->AddInstance(FTransform(rotation, location));
			auto IsmIDBeam = BeamRedISM->AddInstance(FTransform(rotation, location, FVector::ZeroVector));
			auto entity = GetEcsWorld()->entity()
				.set<FlecsIsmRef>({ FriendISM })
				.set<FlecsProjectIsmRef>({ BeamRedISM })
				.set<FlecsISMIndex>({ IsmID })
				.set<FlecsISMBeamIndex>({ IsmIDBeam })
				.set<FlecsHealth>({ 100 })
				.set<FlecsBeamTransform>({ FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector) })
				.set<FlecsUnit>({ FTransform(rotation, locationRand), isFriend, 0, 0, 0, 0, false })
				.child_of<Units>();

		}
		else {
			auto IsmID = EnermyISM->AddInstance(FTransform(rotation, location));
			auto IsmIDBeam = BeamBlueISM->AddInstance(FTransform(rotation, location, FVector::ZeroVector));
			auto entity = GetEcsWorld()->entity()
				.set<FlecsIsmRef>({ EnermyISM })
				.set<FlecsProjectIsmRef>({ BeamBlueISM })
				.set<FlecsISMIndex>({ IsmID })
				.set<FlecsISMBeamIndex>({ IsmIDBeam })
				.set<FlecsHealth>({ 100 })
				.set<FlecsBeamTransform>({ FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector) })
				.set<FlecsUnit>({ FTransform(rotation, locationRand), isFriend, 0, 0, 0, 0, false })
				.child_of<Units>();
		}
	}
}

bool UFlecsSubsystem::Tick(float DeltaTime)
{
	if (ECSWorld) ECSWorld->progress(DeltaTime);
	return true;
}