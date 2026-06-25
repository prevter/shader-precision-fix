<div align="center">
   <a href="https://github.com/prevter/platformer-tweaks">
      <img src="logo.png" alt="Logo" width="80" height="80">
   </a>
   <h3 align="center">Shader Precision Fix</h3>
   <p align="center">
      Say goodbye to 10 bits of precision!
   </p>
</div>

---

## What is this?

This mod eliminates common visual artifacts on mobile devices,
especially when running high-resolution textures. It directly fixes:

* **Color bleeding** across UI and gameplay elements.
* **Ugly white lines/seams** cutting through textures.
* **Slightly misaligned sprites** and rendering jitters.

## How does it work?

*Geometry Dash* runs on **cocos2d-x**, an older game engine that retains legacy code for
compatibility with ancient hardware. Because of this, it enforces `mediump` (16-bit)
and sometimes even `lowp` (10-bit) precision for its OpenGL shaders.

While this saved performance a decade ago, it introduces severe visual artifacts today.
Particularly when using mods like Weebify's **High Graphics on Mobile**.

This mod **overwrites the default shaders to force modern `highp` (32-bit) precision** (same as desktop),
instantly cleaning up the image quality. Modern mobile devices handle 32-bit shader precision
effortlessly, meaning you get a flawless visual experience with **zero noticeable performance impact**.

> [!NOTE]
> This is a temporary standalone release and is planned to be integrated
> into the **High Graphics on Mobile** mod in a future update.

---

## License

This project is licensed under the **MIT License**. See the [LICENSE.md](LICENSE.md) file for details.