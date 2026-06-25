# Shader Precision Fix

## What is this?

This mod eliminates common visual artifacts on mobile devices,
especially when running high-resolution textures. It directly fixes:

* **Color bleeding** across UI and gameplay elements.
* **Ugly white lines/seams** cutting through textures.
* **Slightly misaligned sprites** and rendering jitters.

Consider installing the **High Graphics on Mobile** mod to experience the full benefits of this fix:

<mod:weebify.high-graphics-android>

## How does it work?

*Geometry Dash* runs on **cocos2d-x**, an older game engine that retains legacy code for
compatibility with ancient hardware. Because of this, it enforces `mediump` (16-bit)
and sometimes even `lowp` (10-bit) precision for its OpenGL shaders.

While this saved performance a decade ago, it introduces severe visual artifacts today.
Particularly when using mods like Weebify's **High Graphics on Mobile**.

This mod **overwrites the default shaders to force modern `highp` (32-bit) precision** (same as desktop),
instantly cleaning up the image quality. Modern mobile devices handle 32-bit shader precision
effortlessly, meaning you get a flawless visual experience with **zero noticeable performance impact**.

**Note**: This is a temporary standalone release and is planned to be
integrated into the **High Graphics on Mobile** mod in a future update.
