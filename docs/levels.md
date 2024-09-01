# bitten engine level format

## tiled is nice and all, but can be a bitch at times

I would like to minimize dependices to make it easier to port, not to mention just how badly undocumented it is. Just look at `e_collision.c` for example, fucking gross. Better yet it doesn't even work properly still.... (worse part it worked on raylib iirc)
The other issue is that if i can bearly get collision working with static objects because welp look at that, fucking conversions to get our current location that not even I understand how they worked back in the raylib days (when they actually somewhat worked properly, iirc i even could delete tiles or smth)

## Why not use LDtk

well.. if you looked through `r_render.c` you can find broken code to attempt and do this exact thing, but iirc i didn't like any of the libraries for it. now that I have cpp, i might consider it again though.

## the plan

make something thats easy to use. one of the goals is being able to figure out if we are coliding at different aspect ratios/resolutions and be able to have the character not be dead stuck in the middle of the screen


## ok but what's the catch

well... inorder to do that, i don't want to just remove whats currently working, so I will make a simple map configuration format that will define which format to use.

the goal would be something like this for the time being

```text
version 1
using tiled/LDtk/custom
```

now i dont feel like parsing text, so ill take the easy way out for now

```c
char file[] = {'0x1', '0x0'} // first value is version, second is format (in this case tiled)
```
