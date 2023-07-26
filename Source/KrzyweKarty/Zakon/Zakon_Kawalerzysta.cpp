// Fill out your copyright notice in the Description page of Project Settings.


#include "Zakon_Kawalerzysta.h"
#include "KrzyweKarty/Map/KKMap.h"


TArray<FDirection> AZakon_Kawalerzysta::GetPossibleMoveTiles()
{
	return{
		{-1, 0},
		{1, 0},
		{2, 0},
		{0 , -1},
		{0, 1}
	};
}

