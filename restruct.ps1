# restructure.ps1 - Run this in the root of the Ocean project

function Ensure-Directory($path) {
    if (!(Test-Path $path)) {
        New-Item -ItemType Directory -Path $path | Out-Null
    }
}

# Create target folders
$folders = @(
    "src/core",
    "src/render",
    "src/input",
    "src/camera",
    "src/particles",
    "include",
    "tests",
    "tools",
    "docs",
    "external",
    "build"
)

$folders | ForEach-Object { Ensure-Directory $_ }

# Move core engine files
Move-Item -Path "App.*" -Destination "src/core" -Force
Move-Item -Path "OceanWindow.*" -Destination "src/core" -Force
Move-Item -Path "OceanDevice.*" -Destination "src/core" -Force
Move-Item -Path "OceanSwapChain.*" -Destination "src/core" -Force
Move-Item -Path "OceanPipeline.*" -Destination "src/core" -Force
Move-Item -Path "OceanRenderer.*" -Destination "src/core" -Force

# Move render-related
Move-Item -Path "OceanRenderSystem.*" -Destination "src/render" -Force
Move-Item -Path "OceanModel.*" -Destination "src/render" -Force
Move-Item -Path "OceanGameObject.h" -Destination "src/render" -Force
Move-Item -Path "OceanHashCombine.h" -Destination "src/render" -Force

# Move input
Move-Item -Path "KeyboardListener.*" -Destination "src/input" -Force

# Move camera
Move-Item -Path "OceanPerspectiveCamera.*" -Destination "src/camera" -Force
Move-Item -Path "PerspectiveCamera.*" -Destination "src/camera" -Force

# Move particles
Move-Item -Path "OceanParticleSystem.*" -Destination "src/particles" -Force

# Move other top-level files
Move-Item -Path "main.cpp" -Destination "src" -Force
Move-Item -Path "Ocean.drawio" -Destination "docs" -Force
Move-Item -Path "ProjectReport.md" -Destination "docs" -Force

# Move tiny_obj_loader to external
Ensure-Directory "external/tiny_obj_loader"
Move-Item -Path "tiny_obj_loader.h" -Destination "external/tiny_obj_loader" -Force

# Inform user
Write-Host "`n✅ Project has been restructured successfully!" -ForegroundColor Green
