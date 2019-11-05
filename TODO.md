# Input
 - [ ] hashed Kb/M input from userspace to GLFW callbacks
   - [ ] need to figure out config options, check comments in /InputManager.cpp/hpp files

# Rendering
## Assets
do not worry about file types/structure rn, but consider a nested JSON or YAML config
 - [ ] build asset schema
 - [ ] build asset file layout

## Contexts
### Scenes
do not worry about file types/structure rn, but consider a nested JSON or YAML config
 - [ ] scene build graph of pointers to scene tree
   - [ ] root is always the current scene

# Physics
Not sure where to start here
 - [ ] ability to translate object dependent on 2d mouse coords
   - [ ] ability to select(bind mouse to on_click) on-screen object

# Queues
use boost queues for now

# Threading
 - [ ] utilize priority queueing for "do task near future" vs "do task whenever" 
  - [ ] CPU threadpool for generic task management

# Memory
expect most memory to be managed by the underlying `*Manager` class. Obj gets instantiated, "borrowed" for the lifetime of a scene, and then returned to the Manager. Manager then calls the MemoryManager to ask permission for the operations?

This needs extensive work and planning... if it's even necessary. The idea is that the application resting on the engine should not have to worry about managing things, but instead provide lifetime hints for when an object is spawned, rendered, its mechanics during the lifetime, and then when it should be derendered and/or destroyed.

 - [ ] create queue of destruction
 
# TESTS
 - [ ] add integration/unit tests
   - [ ] figure out a testing framework

## CI
 - [ ] bad commits are automatically rejected if tests fail
   - [ ] CI building and test automation after commits

# Platform Support
worry about versions later

 - [ ] update config for VK(etc...) version arguments
 - [ ] update config for platform
 
 vulkan should be pretty platform agnostic, need to verify w/ docs

## Linux
 - [x] compiles on Linux
 - [x] engine config gets deps

## MacOS
 - [ ] compiles on MacOS

## Win
 - [ ] compiles on Win
