// Fill out your copyright notice in the Description page of Project Settings.


#include "Fraction.h"
#include "SubobjectDataHandle.h"
#include "SubobjectDataSubsystem.h"
#include "Components/ArrowComponent.h"
#include "KrzyweKarty/CharacterHelpersSettings.h"
#include "KrzyweKarty/Cards/KKCharacter.h"


USpawnIndicator::USpawnIndicator()
{
	UStaticMesh* PlatformMesh = UCharacterHelpersSettings::Get()->PlatformMesh.LoadSynchronous();

	BoxExtent = PlatformMesh->GetBoundingBox().GetExtent();
	
	bWantsInitializeComponent = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

AFraction::AFraction()
{
	PrimaryActorTick.bCanEverTick = false;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	ArrowComponent->SetupAttachment(RootComponent);
}

TArray<AKKCharacter*> AFraction::SpawnCharacters()
{
	TArray<AKKCharacter*> SpawnedCharacters;
	
	if(HasAuthority())
	{
		TArray<TObjectPtr<USceneComponent>> SpawnIndicators = ArrowComponent->GetAttachChildren();

		check(SpawnIndicators.Num() == CharactersToSpawn.Num())

		for(int i = 0; i < CharactersToSpawn.Num(); i++)
		{
			FVector SpawnLocation = SpawnIndicators[i]->GetComponentLocation();
			FRotator SpawnRotation = GetActorRotation();

			SpawnedCharacters.Add(GetWorld()->SpawnActor<AKKCharacter>(CharactersToSpawn[i], SpawnLocation, SpawnRotation));
			SpawnedCharacters[i]->Direction = (ID == 0) ? 1 : -1;
		}
	}

	return SpawnedCharacters;
}

AKKCharacter* AFraction::SpawnBase() const
{
	return GetWorld()->SpawnActor<AKKCharacter>(BaseClass, GetActorLocation(), GetActorRotation() + FRotator(0.f, 90.f, 0.f));
}


void AFraction::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RemoveComponents();
	GetWorldTimerManager().SetTimerForNextTick(this, &AFraction::SpawnComponents);
	
}

void AFraction::RemoveComponents()
{
	const TArray<TObjectPtr<USceneComponent>> ExistingComponents = ArrowComponent->GetAttachChildren();
	
	if(CharactersToSpawn.Num() < ExistingComponents.Num())
	{
		TArray<FSubobjectDataHandle> SubobjectData;
		USubobjectDataSubsystem* DataSubsystem = USubobjectDataSubsystem::Get();

		DataSubsystem->GatherSubobjectData(this, SubobjectData);
		FSubobjectDataHandle ParentHandle = SubobjectData[0];

		TArray<FSubobjectDataHandle> ObjectsToDelete;
		for(int i = CharactersToSpawn.Num(); i < ExistingComponents.Num(); i++)
		{
			if(ExistingComponents[i] != nullptr)
			{
				ObjectsToDelete.Add(SubobjectData[i + 2]); //ExistingComponents + this + ArrowComponent;
			}
		}

		FSubobjectDataHandle Dummy;
		DataSubsystem->DeleteSubobjects(ParentHandle, ObjectsToDelete,Dummy, UBlueprint::GetBlueprintFromClass(GetClass()), true);
	}
	
}

void AFraction::SpawnComponents()
{
	const TArray<TObjectPtr<USceneComponent>> ExistingComponents = ArrowComponent->GetAttachChildren();
	
	for(int i = 0; i < CharactersToSpawn.Num(); i++)
	{
		if(!ExistingComponents.IsValidIndex(i))
		{
			Modify();
			
			TArray<FSubobjectDataHandle> SubobjectData;
			USubobjectDataSubsystem* DataSubsystem = USubobjectDataSubsystem::Get();
			DataSubsystem->GatherSubobjectData(this, SubobjectData);

			FText OutFailReason;
			FSubobjectDataHandle ParentHandle = SubobjectData[0];

			if(ParentHandle.IsValid())
			{
				USpawnIndicator* TemplateIndicator = NewObject<USpawnIndicator>(this, FName("SpawnIndicator", i)); // use template to set initial position 
				TemplateIndicator->SetRelativeLocation(FVector(-60.f, i * 100.f, 0.f));
				
				FAddNewSubobjectParams AddSubobjectParams;
				AddSubobjectParams.BlueprintContext = UBlueprint::GetBlueprintFromClass(GetClass());
				AddSubobjectParams.NewClass = USpawnIndicator::StaticClass();
				AddSubobjectParams.ParentHandle = ParentHandle;
				AddSubobjectParams.AssetOverride = TemplateIndicator;
				
				FSubobjectDataHandle Result = DataSubsystem->AddNewSubobject(AddSubobjectParams, OutFailReason);
				
			
				if(Result.IsValid())
				{
					USpawnIndicator* SpawnIndicator = const_cast<USpawnIndicator*>(Result.GetData()->GetObject<USpawnIndicator>());
					
		            SpawnIndicator->AttachToComponent(ArrowComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
			}
		}
	}
}

void AFraction::BeginPlay()
{
	Super::BeginPlay();
	
}



