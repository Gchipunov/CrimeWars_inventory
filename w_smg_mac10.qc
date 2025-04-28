// Created by Crowbar 0.74

$modelname "weapons\w_smg_mac10.mdl"

$bodygroup "studio"
{
	studio "1.smd"
}


$surfaceprop "weapon"

$contents "solid"

$illumposition 1.931 -7.447 3.697

$cdmaterials "models\weapons\w_models\w_smg_mac10\"

$attachment "muzzle" "ValveBiped.weapon_bone" 1.1 5.5 23.1 rotate -90 -90 0
$attachment "2" "ValveBiped.weapon_bone" 0 4.24 12.05 rotate 0 0 0

$cbox 0 0 0 0 0 0

$bbox -3.837 -0.351 -7.012 18.73 4.411 14.406

$definebone "ValveBiped.weapon_bone" "" 0 0 0 0.319539 -0.059817 68.78788 0 0 0 0 0 0
$definebone "ValveBiped.weapon_bone_Clip" "ValveBiped.weapon_bone" -0.310123 -4.371033 5.233141 0 0 0.000001 0 0 0 0 0 0


$sequence "idle" {
	"w_smg_mac10_anims\idle.smd"
	activity "ACT_VM_IDLE" 1
	fadein 0.2
	fadeout 0.2
	snap
	fps 30
}

$sequence "attack1" {
	"w_smg_mac10_anims\attack1.smd"
	activity "ACT_VM_PRIMARYATTACK" 1
	{ event AE_CLIENT_EFFECT_ATTACH 0 "EjectBrass_9mm 2 100" }
	{ event AE_NPC_MUZZLEFLASH 0 "SMG1 MUZZLE" }
	fadein 0.2
	fadeout 0.2
	snap
	fps 30
}

$collisionmodel "w_smg_mac10_physics.smd"
{
	$mass 5
	$inertia 1
	$damping 0
	$rotdamping 0
	$rootbone " "

}

$keyvalues
{
	prop_data 
	{
		"base" "Metal.Small"
	}
}
