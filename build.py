#!/usr/bin/python
import sys
import os
import glob

if sys.version_info[0] != 3 or sys.version_info[1] < 5:
    print("This script requires Python 3.5+")
    exit()

PLUGIN_NAME = 'irtouchmapper.plg'
DEVKITARM = "C:/devkitPro/devkitARM/"
CC = DEVKITARM + "bin/arm-none-eabi-gcc"
OC = DEVKITARM + "bin/arm-none-eabi-objcopy"
LD = DEVKITARM + "bin/arm-none-eabi-ld"

LIBPATH = "-L " + DEVKITARM + "lib/gcc/arm-none-eabi/6.3.0/armv6k/fpu/" + " -L " + DEVKITARM + "arm-none-eabi/lib/armv6k/fpu/" + " -L ./lib/"

COMPILER_ARGS = " -Os -c -s -g -w -march=armv6k -mlittle-endian -mtune=mpcore -mfloat-abi=hard "

def sourceRecursive(pattern):
    s = ""
    for file in glob.iglob(pattern, recursive=True):
        s += file + " "
    return s

def includeRecursive(pattern):
    s = ""
    for file in glob.iglob(pattern, recursive=True):
        s += "-I " + file + " "
    return s

def run(cmd):
    os.system(cmd)

print("Cleaning old files...")
if os.path.exists("./obj"):
    run("rm -r ./obj")

print("Creating ./obj...")
os.makedirs("./obj")

print("Compiling c source...")
run(CC +  " " + includeRecursive('include/**/') + sourceRecursive('source/**/*.c') + COMPILER_ARGS)

print("Compiling assembly...")
run(CC + " " + sourceRecursive('source/**/*.s') + COMPILER_ARGS)

print("Linking...")
run(LD + " " + LIBPATH + " -pie --gc-sections " + sourceRecursive("*.o") + "-T 3ds.ld -lc -lgcc -lctru -lg -lsysbase")

print("Relocating object files to /obj...")
run("cp -r *.o obj/ ")
run("rm *.o")

print("Creating plugin file...")
run(OC + " -O binary a.out " + PLUGIN_NAME + " -S")

if os.path.isfile("a.out"):
    print("Removing a.out...")
    run("rm a.out")
