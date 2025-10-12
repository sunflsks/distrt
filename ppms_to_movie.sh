#!/bin/sh

cd output
ffmpeg -framerate 30 -i scene_%d.ppm -c:v libx264 -pix_fmt yuv420p output.mp4
