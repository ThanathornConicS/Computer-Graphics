#pragma once

#include <pch.h>

struct Material
{
	std::string materialType;
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float shininess;
};	

struct EmeraldMaterial : Material
{
	EmeraldMaterial()
		: Material({ "Emerald", { 0.02f, 0.17f , 0.02f }, { 0.07f, 0.61f , 0.07f }, { 0.63f, 0.72f , 0.63f }, 0.6f })
	{}
};

struct ObsidianMaterial : Material
{
	ObsidianMaterial()
		: Material({ "Obsidian", { 0.05f, 0.05f , 0.06f }, { 0.18f, 0.17f , 0.22f }, { 0.33f, 0.32f , 0.34f }, 0.3f })
	{}
};

struct GoldMaterial : Material
{
	GoldMaterial()
		: Material({ "Gold", { 0.24f, 0.19f , 0.07f }, { 0.75f, 0.6f , 0.22f }, { 0.62f, 0.55f , 0.36f }, 0.4f })
	{}
};

struct CyanPlasticMaterial : Material
{
	CyanPlasticMaterial()
		: Material({ "Cyan Plastic", { 0.0f, 0.1f , 0.06f }, { 0.0f, 0.5f , 0.5f }, { 0.5f, 0.5f , 0.5f }, 0.25f })
	{}
};

const EmeraldMaterial emeraldMaterial;
const ObsidianMaterial obsidianMaterial;
const GoldMaterial goldMaterial;
const CyanPlasticMaterial cyanPlasticMaterial;

enum MaterialPreset 
{
	EMERALD, OBSIDIAN, GOLD, CYANPLASTIC
};