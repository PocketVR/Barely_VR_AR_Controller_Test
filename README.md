# Ungine: The Universal Node-Based Game Engine

Ungine is a high-performance, open-source game engine designed to simplify game development across 2D, 3D, and Virtual Reality platforms. Built on the speed of C++, Ungine leverages a modern node-based architecture and an asynchronous event system to help developers create stable, scalable, and complex games with minimal boilerplate code.

## Key Features
### Performance & Architecture

- **Built on C++:** Harness the raw speed and efficiency of C++ for demanding applications, ensuring high frame rates and quick loading times.
- **Asynchronous Core:** Utilizes an internal Event Loop for efficient task management, enabling highly concurrent and non-blocking I/O.
- **Node-Based Scene Graph:** Every game object—from players and bombs to cameras and scenes—is a node_t, allowing for clear, hierarchical organization and easy inheritance of properties.
- **Smart Memory Management:** Employs a Smart Pointer Garbage Collector system to drastically reduce memory leaks and simplify resource handling compared to traditional C++.

## Developer Experience

Familiar Scripting Style: The architecture offers a syntax and structure that is easy to learn for developers coming from environments like Node.js or similar high-level languages.

- **Built-in Utility:** Includes essential tools such as a JSON parser/stringify system and a powerful RegExp engine.
- **Lifecycle Hooks:** Clear object lifecycle management through powerful callbacks:
- **onLoop(float delta):** For frame-by-frame physics and game logic updates.
- **on2DDraw():** For 2D rendering and drawing operations.
- **on3DDraw():** For 3D rendering and drawing operations.
- **onUIDraw():** For UI rendering and drawing operations.
- **onClose():** For clean-up and resource deallocation.

## Prerequisites

Ungine relies on a C++ compiler and specific dependencies for its runtime and features.

```bash
# Openssl
    🪟: pacman -S mingw-w64-ucrt-x86_64-openssl
    🐧: sudo apt install libssl-dev

# Zlib
    🪟: pacman -S mingw-w64-ucrt-x86_64-zlib
    🐧: sudo apt install zlib1g-dev

#nodepp | https://github.com/NodeppOfficial/nodepp
#raylib | https://github.com/raysan5/raylib
```

## Compile & Run

```bash
g++ -o main main.cpp -lssl -lcrypto -lz -lraylib -L./lib -I./include ; ./main
```

## Hello World

```cpp
#include <nodepp/nodepp.h>
#include <ungine/ungine.h>

using namespace nodepp;
using namespace ungine;

global_t global;

void onMain() {

    engine::start( 800, 600, "Game" );
    engine::set_fps( 60 ); 

    window::set_min_size({ 800, 600 });

    window::mode::set( 
        ungine::window::WINDOW_ALWAYS_RUN |
        ungine::window::WINDOW_RESIZABLE
    );

    node::node_scene([=]( ref_t<node_t> self ){

        self->on2DDraw([=](){
            rl::ClearBackground( rl::BLACK );
            rl::DrawTexture( "hello world", 12, 12, 12, rl::WHITE );
        });

    });

}

```