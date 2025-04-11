#!/bin/bash

if [ -z "$VULKAN_SDK" ]; then
    GLSLC=$(which glslc)
else
    GLSLC="$VULKAN_SDK/bin/glslc"
fi

if ! command -v "$GLSLC" &> /dev/null; then
    echo "glslc not found. Please install Vulkan SDK or add it to PATH."
    exit 1
fi

echo "Using GLSLC: $GLSLC"

for f in shaders/*.vert shaders/*.frag; do
    "$GLSLC" "$f" -o "$f.spv"
done

echo "Done compiling shaders."
