SRC_FILES="src/main.cpp  dependencies/src/glad.c dependencies/include/IMGUI/imgui.cpp dependencies/include/IMGUI/imgui_impl_opengl3.cpp dependencies/include/IMGUI/imgui_impl_glfw.cpp dependencies/include/IMGUI/imgui_draw.cpp dependencies/include/IMGUI/imgui_tables.cpp dependencies/include/IMGUI/imgui_widgets.cpp"
INCLUDE_DIRS="-Isrc -Idependencies/include"
LIB_DIRS="-Ldependencies/lib"
LIBS="-lglfw3 -lopengl32 -lgdi32"
OUT_FILE="build/main.exe" 

g++ -std=c++17 $SRC_FILES $INCLUDE_DIRS $LIB_DIRS $LIBS -o $OUT_FILE

#./build/main.exe