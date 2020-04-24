# RCL busybox
All utils in a single executable

## About VF vector format

VF format is a lossy (loss by quantization) codec for OOMMF and mumax3 OVF vector files.
Binary32 OVF files (OVF for short) are just a header with sizes and attributes in  a human readable format and vectors in IEEE float 32 bit format.
A vector is just a pack of 3 float quantities, so it occupies 96 bit (12 bytes).
I use a new datatype 64 bits wide.
It has:
* 8 bits coding radius exponent (When expressed in float32)
* 23 bits coding the radius mantissa (When expressed in float32)
* 16 bits coding the azimuthal angle (Fixed point , where 0 is 0 and 65535 is close to pi)
* 17 bits coding the polar angle (Fixed point , where 0 is 0 and 131071 is close to 2 pi)

### Why this coding for vectors


## Compiling 

Needed SDL,fftw, zlib and libpng to sucessfull compile

`$ make -j4 busybox.elf`

Make will compile all CPP files into Object and link them into the binary executable.

## Use

`.\busybox.elf help` to list all applications linked within the binary.
This call will produce an output like this:
`0 : any2asciiovf
1 : cacheromfs_test
2 : comsol_tool
3 : embed_tool_tool
4 : extract_cache
5 : filepath_test
6 : fonts_demo
7 : gen_template_tool
8 : get_sky_sizes_tool
9 : include_bundle_tool
10 : minetest_skin_tool
11 : numerical_test
12 : object_counter_test
13 : oommf_test
14 : ovf_diff_tool
15 : png_append_h
16 : png_append_v
17 : png_diff_tool
18 : png_sum_tool
19 : populate_cache
20 : skyfit_tool
21 : skyfit_tool_zoom
22 : test_skyrmion_models
23 : thiele_tool_adj
24 : unit_calculator_tool
25 : vf_check_tool
26 : vf_crush
27 : vf_debug
28 : vf_plot
29 : vf_render
30 : vf_tool
31 : vf_view`

Some of these apps are just a test for some routines but other are powerful tools.

## vf_tool

Convert OVF or Gzipped OVF to VF and VF to binary32 OVF
Uses default compression in VF compression.

## vf_crush

Like optipng or pngcrush performs all possible compressions on the data
