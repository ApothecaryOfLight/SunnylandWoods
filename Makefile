clean : 
	reset
	rm -f objs/InterfaceManager.o objs/MainMenuInterface.o objs/GameInterface.o objs/InputManager.o objs/AnimationManager.o objs/PlayerManager.o objs/EnemyManager.o objs/MapManager.o objs/TimeManager.o objs/CameraManager.o
	rm -f objs/InterfaceManager_win.o objs/MainMenuInterface_win.o objs/GameInterface_win.o objs/InputManager_win.o objs/AnimationManager_win.o objs/PlayerManager.o objs/EnemyManager_win.o objs/MapManager_win.o objs/TimeManager_win.o objs/CameraManager_win.o objs/CollisionManager.o objs/ClickableManager.o objs/LevelManager.o objs/AssetFactory.o
	reset

run-linux : linux
	reset
	cd bin && ./SunnylandWoods_Linux

run-linux-debug: linux
	reset
	cd bin && gdb SunnylandWoods_Linux

run-linux-valgrind : linux
	reset
	cd bin && valgrind --leak-check=yes ./SunnylandWoods_Linux

linux : main.cpp objs/InterfaceManager.o objs/MainMenuInterface.o objs/GameInterface.o objs/InputManager.o objs/AnimationManager.o objs/PlayerManager.o objs/EnemyManager.o objs/MapManager.o objs/TimeManager.o objs/CameraManager.o objs/CollisionManager.o objs/ClickableManager.o objs/LevelManager.o objs/AssetFactory.o
	reset
	g++ -g main.cpp -std=c++11 objs/InterfaceManager.o objs/MainMenuInterface.o objs/GameInterface.o objs/InputManager.o objs/AnimationManager.o objs/PlayerManager.o objs/EnemyManager.o objs/MapManager.o objs/TimeManager.o  objs/CameraManager.o objs/CollisionManager.o objs/ClickableManager.o objs/LevelManager.o objs/AssetFactory.o `sdl2-config --cflags --libs` -lGL -lSDL2_image -o bin/SunnylandWoods_Linux

objs/InterfaceManager.o : src/interface_manager/InterfaceManager.cpp
	reset
	g++ src/interface_manager/InterfaceManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/InterfaceManager.o

objs/GameInterface.o : src/game_interface/GameInterface.cpp
	reset
	g++ src/game_interface/GameInterface.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/GameInterface.o

objs/MainMenuInterface.o : src/main_menu_interface/MainMenuInterface.cpp
	reset
	g++ src/main_menu_interface/MainMenuInterface.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/MainMenuInterface.o

objs/InputManager.o : src/input_manager/InputManager.cpp
	reset
	g++ src/input_manager/InputManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/InputManager.o

objs/AnimationManager.o : src/animation_manager/AnimationManager.cpp
	reset
	g++ src/animation_manager/AnimationManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/AnimationManager.o

objs/PlayerManager.o : src/player_manager/PlayerManager.cpp
	reset
	g++ src/player_manager/PlayerManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/PlayerManager.o

objs/EnemyManager.o : src/enemy_manager/EnemyManager.cpp
	reset
	g++ src/enemy_manager/EnemyManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/EnemyManager.o

objs/MapManager.o : src/map_manager/MapManager.cpp
	reset
	g++ src/map_manager/MapManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/MapManager.o

objs/TimeManager.o : src/time_manager/TimeManager.cpp
	reset
	g++ src/time_manager/TimeManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/TimeManager.o

objs/CameraManager.o : src/camera_manager/CameraManager.cpp
	reset
	g++ src/camera_manager/CameraManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/CameraManager.o

objs/CollisionManager.o : src/collision_manager/CollisionManager.cpp
	reset
	g++ src/collision_manager/CollisionManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/CollisionManager.o

objs/ClickableManager.o : src/clickable_manager/ClickableManager.cpp
	reset
	g++ src/clickable_manager/ClickableManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/ClickableManager.o

objs/LevelManager.o : src/level_manager/LevelManager.cpp
	reset
	g++ src/level_manager/LevelManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/LevelManager.o

objs/AssetFactory.o : src/asset_factory/AssetFactory.cpp
	reset
	g++ src/asset_factory/AssetFactory.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/AssetFactory.o


## GTest - Runs all GoogleTest unit tests.
linux-gtest : main_gtest.cpp objs/CameraManager.o objs/CameraManager_gtest.o
	reset
	g++ main_gtest.cpp objs/CameraManager.o objs/CameraManager_gtest.o -I/usr/local/include/gtest -g -lgtest -lgtest_main -lpthread -o bin/SunnylandWoods_gtest
	cd bin && ./SunnylandWoods_gtest --gtest_output="xml:./SunnylandWoods_gtest.xml"

objs/CameraManager_gtest.o : src/camera_manager_gtest/CameraManager_gtest.cpp
	reset
	g++ src/camera_manager_gtest/CameraManager_gtest.cpp -c -o objs/CameraManager_gtest.o


#windows target
run-win-dynamic : win-dynamic
	reset
	cd bin && wine SunnylandWoods_win32.exe

win-dynamic : main.cpp objs/InterfaceManager_win.o objs/MainMenuInterface_win.o objs/GameInterface_win.o objs/InputManager_win.o objs/AnimationManager_win.o objs/PlayerManager_win.o objs/EnemyManager_win.o objs/MapManager_win.o objs/TimeManager_win.o objs/CameraManager_win.o objs/CollisionManager_win.o objs/ClickableManager_win.o objs/LevelManager_win.o objs/AssetFactory_win.o
	reset
	x86_64-w64-mingw32-g++ main.cpp -std=c++11 objs/InterfaceManager_win.o objs/MainMenuInterface_win.o objs/GameInterface_win.o objs/InputManager_win.o objs/AnimationManager_win.o objs/PlayerManager_win.o objs/EnemyManager_win.o objs/MapManager_win.o objs/TimeManager_win.o objs/CameraManager_win.o objs/CollisionManager_win.o objs/ClickableManager_win.o objs/LevelManager_win.o objs/AssetFactory_win.o -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/SDL2/ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ `"libSDL/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o bin/SunnylandWoods_win32.exe

objs/InterfaceManager_win.o : src/interface_manager/InterfaceManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/interface_manager/InterfaceManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/InterfaceManager_win.o

objs/GameInterface_win.o : src/game_interface/GameInterface.cpp
	reset
	x86_64-w64-mingw32-g++ src/game_interface/GameInterface.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/GameInterface_win.o

objs/MainMenuInterface_win.o : src/main_menu_interface/MainMenuInterface.cpp
	reset
	x86_64-w64-mingw32-g++ src/main_menu_interface/MainMenuInterface.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/MainMenuInterface_win.o

objs/InputManager_win.o : src/input_manager/InputManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/input_manager/InputManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/InputManager_win.o

objs/AnimationManager_win.o : src/animation_manager/AnimationManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/animation_manager/AnimationManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/AnimationManager_win.o

objs/PlayerManager_win.o : src/player_manager/PlayerManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/player_manager/PlayerManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/PlayerManager_win.o

objs/EnemyManager_win.o : src/enemy_manager/EnemyManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/enemy_manager/EnemyManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/EnemyManager_win.o

objs/MapManager_win.o : src/map_manager/MapManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/map_manager/MapManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/MapManager_win.o

objs/TimeManager_win.o : src/time_manager/TimeManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/time_manager/TimeManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/TimeManager_win.o

objs/CameraManager_win.o : src/camera_manager/CameraManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/camera_manager/CameraManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/CameraManager_win.o

objs/CollisionManager_win.o : src/collision_manager/CollisionManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/collision_manager/CollisionManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/CollisionManager_win.o

objs/ClickableManager_win.o : src/clickable_manager/ClickableManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/clickable_manager/ClickableManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/ClickableManager_win.o

objs/LevelManager_win.o : src/level_manager/LevelManager.cpp
	reset
	x86_64-w64-mingw32-g++ src/level_manager/LevelManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/LevelManager_win.o

objs/AssetFactory_win.o : src/asset_factory/AssetFactory.cpp
	reset
	x86_64-w64-mingw32-g++ src/level_manager/AssetFactory.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/AssetFactory_win.o
