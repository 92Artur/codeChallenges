# CMakeList.txt: CMake-Projektdatei der obersten Ebene. Führen Sie eine globale Konfiguration durch,
# und schließen Sie hier Unterprojekte ein.
#
cmake_minimum_required (VERSION 3.0)

project ("basicTasks")

# Schließen Sie Unterprojekte ein.
add_subdirectory ("basicTasks")

target_compile_features(basicTasks PRIVATE cxx_range_for)

