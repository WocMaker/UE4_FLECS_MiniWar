
#include "FriendActor.h"

void AFriendActor::SteerFunc(float DeltaTime)
{
	float steerCD = 2;
	if (SteerTimer > steerCD) {
		Vx = FMath::RandRange(-100, 100);
		Vy = IsFriend ? -200 : 200;
		Vz = FMath::RandRange(-200, 200);
		SteerTimer = 0;
	}
	else {
		SteerTimer += DeltaTime;
	}
}

void AFriendActor::MoveFunc(float DeltaTime)
{
	FVector movementDirection = FVector(Vx, Vy, Vz).GetSafeNormal();
	FVector newlocation = GetActorLocation() + FVector(Vx * DeltaTime, Vy * DeltaTime, Vz * DeltaTime);
	FRotator newRotation = movementDirection.ToOrientationRotator();
	SetActorTransform(FTransform(newRotation, newlocation, FVector(1,1,1)));
}

void AFriendActor::BoundFunc(float DeltaTime)
{
	float maxPosX = 3000;
	float maxPosY = 5000;
	float maxPosZ = 4000;
	FVector NewLocation = GetActorLocation();
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
	SetActorLocation(NewLocation);
}

void AFriendActor::HideDeadFunc(float DeltaTime)
{
	if (Health >= 0)
		return;
	SetActorScale3D(FVector::ZeroVector);
}

// Sets default values
AFriendActor::AFriendActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SteerTimer = 0;
	IsFriend = true;
	Health = 100;
	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMesh"));
	RootComponent = MyStaticMesh;

}

void AFriendActor::SetFriend(bool bisFriend)
{
	IsFriend = bisFriend;
}

bool AFriendActor::GetTeam()
{
	return IsFriend;
}
// Called when the game starts or when spawned
void AFriendActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFriendActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SteerFunc(DeltaTime);
	MoveFunc(DeltaTime);
	BoundFunc(DeltaTime);
	HideDeadFunc(DeltaTime);
}

void AFriendActor::SetStaticMesh(UStaticMesh* InStaticMesh)
{
	if (MyStaticMesh && InStaticMesh)
	{
		MyStaticMesh->SetStaticMesh(InStaticMesh);
	}
}