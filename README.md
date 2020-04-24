# RCL busybox
All utils in a single executable.
I chose this way because I had to run this collection of programs on several computers. Update is as easy as overwritting a file. Since all programs share code, binaries will have repeated code. Changing a part may lead to change tens of binaries.
If using a dynamic library, a change means that I have to update a file. So I took this as the best solution.

To add or remove an app, is not necessary to change the existing source files. Just link the apps with the busybox core and library depencencies and that's all.

The way to invoke a command is `$ ./busybox.elf <command> <command args>`
or rename/link executable to the name of the command:
`$ ln busybox.elf vf_view -s`
`./vf_view file.vf`

Another way to compile this code is to make a launcher and a dynamic library. To update you have to replace dynamic library.

This program contains first implementation of VF codec for Vectorial Fields used in micromagnetic codes like NIST's OOMMF or Mumax3.
VF format is much more efficient than any OVF subformat compressed with deflate/inflate algorithm.

## About VF vector format

VF format is a lossy (loss by quantization) codec for OOMMF and mumax3 OVF vector files.
Binary32 OVF files (OVF for short) are just a header with sizes and attributes in  a human readable format and vectors in IEEE float 32 bit format.
A vector is just a pack of 3 float quantities, so it occupies 96 bit (12 bytes).
I use a new datatype 64 bits wide (`rpolar`). 
It has:
* 8 bits coding radius exponent (When expressed in float32)
* 23 bits coding the radius mantissa (When expressed in float32)
* 16 bits coding the azimuthal angle (Fixed point , where 0 is 0 and 65535 is close to pi)
* 17 bits coding the polar angle (Fixed point , where 0 is 0 and 131071 is close to 2 pi)

Note that sign bit is deprecated since sign of three quantities of the vector are in the azimuthal angle and polar angle.

### Why this coding for vectors?

When you 'cook' a field, let's say a domain wall, an isolated skyrmion or a vortex, if vectors in a region are pointing up, their coords are just (0,0,1), because data come from an analytical formula and they can be compressed efectively using Gzip or other standard techniques. But when data comes from a real simulation, if a zone looks like pointing up with naked eye, vectors are not as nice as before, they look like (3.423523e-8,7.3143e-9,0.9999999). What do we have? Two floats fill of garbage (mantissa noise). Coding to `rpolar`, the result is (rad=1,theta=0±2,phi=0±2). Field has smooth variations between neighbours, which means that it will be more easy to compress.
Some data is destroyed in this process, but doesn't carry relevant physical data. Most of the information is carried in the orientation (dot product) and it's stored with about 4~5 decimal places. Radius is stored with a normal float32 number.

### OVF header storage (Metadata)

Header is stored just dumping key-pair values from a `std::map<std::string,std::string>` to a `std::vector<unsigned char>`, dumping a 32bit `unsigned int` with the lenght of the string and after these 4 bytes, all the bytes from the string.

For example , key (string of lenght 18) and val (string of lenght 21) dump to a 47 byte long vector.
Once all key-value pairs are dumped to vector, it is stored in the VF file compressed by zlib.

OVF header have about 20 pairs and VF have some _extensions_ (new keys), so about 30 pairs.
This data is stored in a regular C++ `map<string,string>` when loaded and packed all together in files because using a DB system is cracking a nut with a sledgehammer.
Numerical values are stored converted to a regular ASCII human-readable string. 

### Field storage 

Field can be just converted to a sequence of bytes and compressed with zlib or it can be conditioned before compression to achieve maximum deflate efficiency.

Conditioning is just applying some filters that given a bunch of data and a parameter, return same data but swapping bits or bytes depending on the parameter. That parameter is stored in header, where key is the name of the filter and value is the parameter. At a cost of few bytes in header, it saves a significant fraction of the data in the field.

### Recovering data

Process for loading data is the following:

* Read VF signature
* Read VF header
* Inflate header to a `vector`
* Populate `map<string,string>` with strings in `vector`
* Convert relevant numerical values to `int` or `float` (Hard-coded depending of key)
* Load field data and inflate
* Apply filter inverse operation
* Convert polar(64 bit) to cartesian (3x32bit)

## Compiling 

Needed SDL,fftw, zlib and libpng to sucessfull compile

`$ make -j4 busybox.elf`

Make will compile all CPP files into Object and link them into the binary executable.

## Use

`.\busybox.elf help` to list all applications linked within the binary.
This call will produce an output like this:
`0 : any2asciiovf`
`1 : cacheromfs_test`
`2 : comsol_tool`
`3 : embed_tool_tool`
`4 : extract_cache`
`5 : filepath_test`
`6 : fonts_demo`
`7 : gen_template_tool`
`8 : get_sky_sizes_tool`
`9 : include_bundle_tool`
`10 : minetest_skin_tool`
`11 : numerical_test`
`12 : object_counter_test`
`13 : oommf_test`
`14 : ovf_diff_tool`
`15 : png_append_h`
`16 : png_append_v`
`17 : png_diff_tool`
`18 : png_sum_tool`
`19 : populate_cache`
`20 : skyfit_tool`
`21 : skyfit_tool_zoom`
`22 : test_skyrmion_models`
`23 : thiele_tool_adj`
`24 : unit_calculator_tool`
`25 : vf_check_tool`
`26 : vf_crush`
`27 : vf_debug`
`28 : vf_plot`
`29 : vf_render`
`30 : vf_tool`
`31 : vf_view`

Some of these apps are just a test for some routines but other are powerful tools.

## vf_tool

Convert OVF or Gzipped OVF to VF and VF to binary32 OVF
Uses default compression in VF compression.

## vf_crush

Like optipng or pngcrush performs all possible compressions on the data and keeps the best parameters.
Normal run of vf_crush is quite slow and with multilayer files (z>1) may take too much time.
To have a quick encoding and fair results , use `quickpy` before files. 

For OVF/Gzip+OVF  files, OVF is loaded and a VF file is written.
**VF files will be crushed and then overwritten** 

`$ ./busybox.elf vf_crush quickpy file.ovf`

## any2asciiovf 

Take OVF/Gzipped OVF/VF and convert to plain ASCII , human readable OVF.

## vf_debug

Take VF file and print OVF header. (Only reads Header. Useful to avoid VF to OVF conversion. Useful for bash-scripting)

## vf_plot

Load OVF/VF file and write a Mumax3-like PNG of the field.

## vf_view

SDL app to load OVF/VF files and view plot on screen.
[1-9] to change sort of plot HSV absolute/relative/normalized/france(x,y,z)/gray(x,y,z)
+,- to change layer
Arrows to advance in files
E to apply normpeak (find peak and normalize field)
R to reload current file
Esc or Q to exit

## png_append_h and png_append_v

Join two or more PNGs. Like imagemagick does , but using this code.

## Multithreading

This code doesn't have any sort of multi-core optimization, but you can juice your CPU just using xargs.

For an Intel i7 with 4 physical cores and 2 threads per core you can invoke:
`ls *.ovf | xargs -P 8 -n 1 ./busybox.elf vf_crush quickpy`
