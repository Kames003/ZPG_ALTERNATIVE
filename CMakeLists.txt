cmake_minimum_required(VERSION 3.29)
project(untitled2)
set(CMAKE_CXX_STANDARD 11)  # ✅ Zmeň na C++11 (stačí pre projekt)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========================================
# HOMEBREW SETUP
# ========================================
execute_process(
        COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Homebrew prefix: ${HOMEBREW_PREFIX}")

# ========================================
# GLFW
# ========================================
find_path(GLFW_INCLUDE_DIR
        NAMES GLFW/glfw3.h
        HINTS ${HOMEBREW_PREFIX}/include
)
if (GLFW_INCLUDE_DIR)
    message(STATUS "Found GLFW include directory: ${GLFW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLFW include directory")
endif()

find_library(GLFW_LIB
        NAMES glfw glfw3
        HINTS ${HOMEBREW_PREFIX}/lib
)
if (GLFW_LIB)
    message(STATUS "Found GLFW library: ${GLFW_LIB}")
else()
    message(FATAL_ERROR "Could not find GLFW library")
endif()

# ========================================
# GLM (header-only)
# ========================================
find_path(GLM_INCLUDE_DIR
        NAMES glm/glm.hpp
        HINTS ${HOMEBREW_PREFIX}/include
)
if (GLM_INCLUDE_DIR)
    message(STATUS "Found GLM include directory: ${GLM_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLM include directory")
endif()

# ========================================
# GLEW
# ========================================
find_path(GLEW_INCLUDE_DIR
        NAMES GL/glew.h
        HINTS ${HOMEBREW_PREFIX}/include
)
if (GLEW_INCLUDE_DIR)
    message(STATUS "Found GLEW include directory: ${GLEW_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find GLEW include directory")
endif()

find_library(GLEW_LIB
        NAMES GLEW glew
        HINTS ${HOMEBREW_PREFIX}/lib
)
if (GLEW_LIB)
    message(STATUS "Found GLEW library: ${GLEW_LIB}")
else()
    message(FATAL_ERROR "Could not find GLEW library")
endif()

# ========================================
# ASSIMP (NOVÉ!)
# ========================================
find_path(ASSIMP_INCLUDE_DIR
        NAMES assimp/Importer.hpp
        HINTS ${HOMEBREW_PREFIX}/include
)
if (ASSIMP_INCLUDE_DIR)
    message(STATUS "Found Assimp include directory: ${ASSIMP_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Could not find Assimp include directory. Install with: brew install assimp")
endif()

find_library(ASSIMP_LIB
        NAMES assimp
        HINTS ${HOMEBREW_PREFIX}/lib
)
if (ASSIMP_LIB)
    message(STATUS "Found Assimp library: ${ASSIMP_LIB}")
else()
    message(FATAL_ERROR "Could not find Assimp library. Install with: brew install assimp")
endif()

# ========================================
# SOURCE FILES (BEZ DUPLICÍT!)
# ========================================
set(SOURCES
        # Main
        src/main.cpp

        # Application
        src/Application.cpp
        src/Application.h

        # Shader system
        src/Shader.cpp
        src/Shader.h
        src/ShaderProgram.cpp
        src/ShaderProgram.h

        # Models
        src/Model.cpp
        src/Model.h

        # Transformations
        src/Transformation.cpp
        src/Transformation.h
        src/TransformationTranslate.cpp
        src/TransformationTranslate.h
        src/TransformationRotate.cpp
        src/TransformationRotate.h
        src/TransformationScale.cpp
        src/TransformationScale.h
        src/TransformationComposite.cpp
        src/TransformationComposite.h

        # Objects & Scenes
        src/DrawableObject.cpp
        src/DrawableObject.h
        src/Camera.cpp
        src/Camera.h
        src/models/plane.h
        src/models/lamp.h
        src/models/bench.h
        src/Light.cpp
        src/Light.h
        src/models/triangle.h
        src/Subject.h
        src/Subject.cpp
        src/AbstractModel.cpp
        src/AbstractModel.h
        src/SphereModel.cpp
        src/SphereModel.h
        src/VAO.cpp
        src/VAO.h
        src/VBO.cpp
        src/VBO.h
        src/TreeModel.cpp
        src/TreeModel.h
        src/BushModel.cpp
        src/BushModel.h
        src/PlainModel.cpp
        src/PlainModel.h
        src/GiftModel.cpp
        src/GiftModel.h
        src/BenchModel.cpp
        src/BenchModel.h
        src/FileReader.cpp
        src/FileReader.h
        src/LoadedModel.cpp
        src/LoadedModel.h
        src/PointLight.cpp
        src/PointLight.h
        src/DirectionalLight.cpp
        src/DirectionalLight.h
        src/SpotlightCamera.cpp
        src/SpotlightCamera.h
        src/Spotlight.cpp
        src/Spotlight.h
        src/LightManager.cpp
        src/LightManager.h
        src/ObjectManager.cpp
        src/ObjectManager.h
        src/ShaderProgramManager.cpp
        src/ShaderProgramManager.h
        src/AbstractScene.cpp
        src/AbstractScene.h
        src/Window.cpp
        src/Window.h
        src/VertexShader.cpp
        src/VertexShader.h
        src/FragmentShader.cpp
        src/FragmentShader.h
        src/Callback.cpp
        src/Callback.h
        src/MinimalForestScene.h
        src/MinimalForestScene.cpp
        src/SceneManager.cpp
        src/SceneManager.h
)

# ========================================
# EXECUTABLE
# ========================================
add_executable(untitled2 ${SOURCES})

# ========================================
# INCLUDE DIRECTORIES
# ========================================
target_include_directories(untitled2 PRIVATE
        ${GLFW_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${ASSIMP_INCLUDE_DIR}          # ✅ ASSIMP!
        ${CMAKE_SOURCE_DIR}/src        # ✅ src/ priečinok
        ${CMAKE_SOURCE_DIR}/src/models # ✅ models/ priečinok!
)

# ========================================
# LINKING
# ========================================
target_link_libraries(untitled2 PRIVATE
        ${GLFW_LIB}
        ${GLEW_LIB}
        ${ASSIMP_LIB}                  # ✅ ASSIMP!
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework OpenGL"
        "-framework CoreFoundation"
        "-lobjc"
)

# Potlačenie deprecated warnings
target_compile_definitions(untitled2 PRIVATE GL_SILENCE_DEPRECATION)
# ========================================
# COPY SHADERS TO BUILD DIRECTORY
# ========================================
# Skopíruje Shaders priečinok do build priečinka pri každom buildu
add_custom_command(TARGET untitled2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/Shaders
        $<TARGET_FILE_DIR:untitled2>/Shaders
        COMMENT "📁 Copying shaders to build directory..."
)

# Alternatívne: Skopíruj pri konfigurácii (raz)
file(COPY ${CMAKE_SOURCE_DIR}/src/Shaders
        DESTINATION ${CMAKE_BINARY_DIR})