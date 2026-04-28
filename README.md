# PPM is fun

## basics

```txt
P3
640 480
255
255 0 0
0 255 0
0 0 255
255 255 0
...
```

## Gif

make a gif from a sequence of ppm files:

```bash
ffmpeg -framerate 30 -i frame_%04d.ppm -vf "scale=600:600" -c:v libx264 -pix_fmt yuv420p output.gif
```

It's smooth and fast.

## Fun applicaton

- show moon phases
  - show a sphere with a light source moving around it
  - animate the phases of the moon
  - show the earth and the moon orbiting around it
- show a random image
- show a wave pattern
