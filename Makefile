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

objs/InterfaceManager.o : InterfaceManager.cpp
	reset
	g++ InterfaceManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/InterfaceManager.o

objs/GameInterface.o : GameInterface.cpp
	reset
	g++ GameInterface.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/GameInterface.o

objs/MainMenuInterface.o : MainMenuInterface.cpp
	reset
	g++ MainMenuInterface.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/MainMenuInterface.o

objs/InputManager.o : InputManager.cpp
	reset
	g++ InputManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/InputManager.o

objs/AnimationManager.o : AnimationManager.cpp
	reset
	g++ AnimationManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/AnimationManager.o

objs/PlayerManager.o : PlayerManager.cpp
	reset
	g++ PlayerManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/PlayerManager.o

objs/EnemyManager.o : EnemyManager.cpp
	reset
	g++ EnemyManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/EnemyManager.o

objs/MapManager.o : MapManager.cpp
	reset
	g++ MapManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -lGL -lSDL2_image -o objs/MapManager.o

objs/TimeManager.o : TimeManager.cpp
	reset
	g++ TimeManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/TimeManager.o

objs/CameraManager.o : CameraManager.cpp
	reset
	g++ CameraManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/CameraManager.o

objs/CollisionManager.o : CollisionManager.cpp
	reset
	g++ CollisionManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/CollisionManager.o

objs/ClickableManager.o : ClickableManager.cpp
	reset
	g++ ClickableManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/ClickableManager.o

objs/LevelManager.o : LevelManager.cpp
	reset
	g++ LevelManager.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/LevelManager.o

objs/AssetFactory.o : AssetFactory.cpp
	reset
	g++ AssetFactory.cpp -std=c++11 -g -c `sdl2-config --cflags --libs` -o objs/AssetFactory.o

run-win-dynamic : win-dynamic
	reset
	cd bin && wine SunnylandWoods_win32.exe

win-dynamic : main.cpp -std=c++11 objs/InterfaceManager_win.o objs/MainMenuInterface_win.o objs/GameInterface_win.o objs/InputManager_win.o objs/AnimationManager_win.o objs/PlayerManager_win.o objs/EnemyManager_win.o objs/MapManager_win.o objs/TimeManager_win.o objs/CameraManager_win.o objs/CollisionManager_win.o objs/ClickableManager_win.o objs/LevelManager_win.o objs/AssetFactory_win.o
	reset
	x86_64-w64-mingw32-g++ main.cpp -std=c++11 objs/InterfaceManager_win.o objs/MainMenuInterface_win.o objs/GameInterface_win.o objs/InputManager_win.o objs/AnimationManager_win.o objs/PlayerManager_win.o objs/EnemyManager_win.o objs/MapManager_win.o objs/TimeManager_win.o objs/CameraManager_win.o objs/CollisionManager_win.o objs/ClickableManager_win.o objs/LevelManager_win.o objs/AssetFactory_win.o -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/SDL2/ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ `"libSDL/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o bin/SunnylandWoods_win32.exe

objs/InterfaceManager_win.o : InterfaceManager.cpp
	reset
	x86_64-w64-mingw32-g++ InterfaceManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/InterfaceManager_win.o

objs/GameInterface_win.o : GameInterface.cpp
	reset
	x86_64-w64-mingw32-g++ GameInterface.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/GameInterface_win.o

objs/MainMenuInterface_win.o : MainMenuInterface.cpp
	reset
	x86_64-w64-mingw32-g++ MainMenuInterface.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/MainMenuInterface_win.o

objs/InputManager_win.o : InputManager.cpp
	reset
	x86_64-w64-mingw32-g++ InputManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/InputManager_win.o

objs/AnimationManager_win.o : AnimationManager.cpp
	reset
	x86_64-w64-mingw32-g++ AnimationManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/AnimationManager_win.o

objs/PlayerManager_win.o : PlayerManager.cpp
	reset
	x86_64-w64-mingw32-g++ PlayerManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/PlayerManager_win.o

objs/EnemyManager_win.o : EnemyManager.cpp
	reset
	x86_64-w64-mingw32-g++ EnemyManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/EnemyManager_win.o

objs/MapManager_win.o : MapManager.cpp
	reset
	x86_64-w64-mingw32-g++ MapManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/MapManager_win.o

objs/TimeManager_win.o : TimeManager.cpp
	reset
	x86_64-w64-mingw32-g++ TimeManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/TimeManager_win.o

objs/CameraManager_win.o : CameraManager.cpp
	reset
	x86_64-w64-mingw32-g++ CameraManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/CameraManager_win.o

objs/CollisionManager_win.o : CollisionManager.cpp
	reset
	x86_64-w64-mingw32-g++ CollisionManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/CollisionManager_win.o

objs/ClickableManager_win.o : ClickableManager.cpp
	reset
	x86_64-w64-mingw32-g++ ClickableManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/ClickableManager_win.o

objs/LevelManager_win.o : LevelManager.cpp
	reset
	x86_64-w64-mingw32-g++ LevelManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/LevelManager_win.o

objs/AssetFactory_win.o : AssetFactory.cpp
	reset
	x86_64-w64-mingw32-g++ LevelManager.cpp -std=c++11 -static-libgcc -static-libstdc++ -I/usr/local/cross-tools/x86_64-w64-mingw32/include/ -I/Inlucde/SDL2_win/ -g -c `"Include/sdl2-config" --libs` -L"libSDL/" -L/usr/local/cross-tools/x86_64-w64-mingw32/lib/ -lSDL2_image -lopengl32 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -o objs/AssetFactory_win.o
