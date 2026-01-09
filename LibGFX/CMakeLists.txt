# LibGFX/CMakeLists.txt

# Static Library erstellen
add_library(LibGFX STATIC
    LibGFX.cpp
    LibGFX.h
 "VkRenderer.h" "VkRenderer.cpp" "QueueFamilyIndices.h"  "SwapChainSupportDetails.h" "SwapchainInfo.h"  "DepthBuffer.h" "RenderPass.h" "DefaultRenderPass.h" "DefaultRenderPass.cpp" "DescriptorSetLayoutBuilder.h" "DescriptorSetLayoutBuilder.cpp")

# GLFW mit der Library linken
target_link_libraries(LibGFX 
    PUBLIC 
        glfw
        Vulkan::Vulkan
)

# Include-Verzeichnisse
target_include_directories(LibGFX 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# C++ Standard setzen
target_compile_features(LibGFX 
    PUBLIC cxx_std_17
)