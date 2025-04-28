#!/bin/bash
# This script is used to build the project using Conan and CMake.

# Exit immediately if a command exits with a non-zero status.
set -e

# Define the build directory name
BUILD_DIR="build"
# Get the directory where the script is located (project root)
PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

echo "--- Cleaning up previous build directory: ${PROJECT_ROOT}/${BUILD_DIR} ---"
rm -rf "${PROJECT_ROOT}/${BUILD_DIR}"

echo "--- Creating build directory: ${PROJECT_ROOT}/${BUILD_DIR} ---"
mkdir -p "${PROJECT_ROOT}/${BUILD_DIR}"

# Navigate into the build directory
cd "${PROJECT_ROOT}/${BUILD_DIR}"
echo "--- Current directory: $(pwd) ---"

echo "--- Running Conan install ---"
conan install .. --output-folder=. --generator=CMakeDeps --generator=CMakeToolchain --build=missing

echo "--- Sourcing Conan environment ---"
. ./conanbuild.sh

# echo "--- Patching CMakePresets.json to use version 4 ---"
# sed -i 's/"version": 3/"version": 4/' "${PROJECT_ROOT}/${BUILD_DIR}/CMakePresets.json"

# echo "--- Ensuring CMakeUserPresets.json is version 4 ---"
# cat > "${PROJECT_ROOT}/CMakeUserPresets.json" << EOL
# {
#     "version": 4,
#     "vendor": {
#         "conan": {}
#     },
#     "include": [
#         "${BUILD_DIR}/CMakePresets.json"
#     ]
# }
# EOL

echo "--- Configuring project with CMake using preset 'conan-release' ---"
cmake --preset conan-release -S "${PROJECT_ROOT}"

echo "--- Building project with CMake using preset 'conan-release' ---"
cmake --build --preset conan-release

echo "--- Running tests ---"
ctest --preset conan-release

echo "--- Deactivating Conan environment ---"
. ./deactivate_conanbuild.sh

echo "--- Build finished successfully in ${PROJECT_ROOT}/${BUILD_DIR} ---"

# Optional: Go back to the original directory
cd "${PROJECT_ROOT}"