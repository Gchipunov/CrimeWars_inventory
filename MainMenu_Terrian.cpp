// displacement.cpp
#include "cbase.h"
#include "displacement.h"

// Generates a terrain for a main menu background
void GenerateMainMenuTerrain()
{
    // Initialize a displacement surface
    CDisplacementSurface* pSurface = new CDisplacementSurface();
    pSurface->Init();

    // Set dimensions - larger for a dramatic main menu vista (4K resolution friendly)
    pSurface->SetSize(4096, 4096); // Increased resolution for modern displays

    // Create the displacement object
    CDisplacement* pDisplacement = new CDisplacement();
    pDisplacement->Init(pSurface);

    // Use a high-quality texture blend for a natural, visually appealing look
    pDisplacement->SetTexture("nature/blendrockgrass001a"); // Rock and grass blend for variety
    pDisplacement->SetDetailTexture("detail/noise_detail_01"); // Adds fine detail for close-up rendering

    // Sculpt the terrain with layered features for a main menu backdrop
    // Base layer: Rolling hills
    pDisplacement->Sculpt(0, 0, 500, 500, 0.5f); // Broad, smooth elevation across the map

    // Mid layer: Central peak for focus
    pDisplacement->Sculpt(1800, 1800, 300, 300, 2.0f); // Taller central mountain

    // Detail layer: Erosion-like cuts and ridges
    pDisplacement->Sculpt(1600, 1600, 50, 50, -0.8f); // Small negative cuts for realism
    pDisplacement->Sculpt(2000, 2000, 80, 80, 1.2f);  // Positive ridges for contrast

    // Apply smoothing to avoid harsh edges
    pDisplacement->Smooth(2.0f); // Moderate smoothing for natural contours

    // Add a water plane for reflection (main menu polish)
    pSurface->AddWaterPlane(0.0f, "water/water_riverbed01"); // Flat water at base height

    // Optimize the surface for rendering performance
    pSurface->Optimize();

    // Save the terrain file
    pSurface->Save("mainmenu_terrain_2025.dsp");

    // Cleanup
    delete pDisplacement;
    delete pSurface;
}
