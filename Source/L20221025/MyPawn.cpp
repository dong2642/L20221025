// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/FloatingPawnMovement.h"
#include "Engine/StaticMesh.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//박스 만들기
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	//박스에 바디
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	
	// Body 매쉬 불러오고, if로 성공 여부를 물어봄
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	//바디에 레프트
	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	//바디에 라이트
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	//프로펠러 붙이기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Right->SetStaticMesh(SM_Propeller.Object);
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetRelativeLocation(FVector(36.269119f, 21.026630f, 0.310581f));
		Left->SetRelativeLocation(FVector(36.269119f, -21.026630f, 0.310581f));

	}

	// 박스에 스프링암 붙이기
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm -> SetupAttachment(Box);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	//스프링암에 카메라 붙이기
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//박스에 에로우를 붙이고 위치를 X축 +80에 
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(80.f, 0, 0));

	//엑터의 무브먼트 설정. 속도는 100
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = MoveSpeed;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//프로펠러 회전
	Left->AddLocalRotation(FRotator(0, 0, PropellerRoationSpeed * DeltaTime));
	Right->AddLocalRotation(FRotator(0, 0, PropellerRoationSpeed * DeltaTime));

	//AddActorLocalOffset(FVector(100.0f * DeltaTime, 0, 0));
	//엑터 앞으로 이동
	AddMovementInput(GetActorForwardVector());

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);

}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AActor>(AActor::StaticClass(),
		GetActorLocation(),
		GetActorRotation());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("StaticMesh'/Game/P38/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{

	}
}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		0,
		0
	));
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0, 0, Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())
	));
}

