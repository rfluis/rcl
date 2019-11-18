all: busybox.elf
	@echo Expecify an objetive:

#CPPFLAGS = -fPIC -O3 -s -std=c++14 -nodefaultlibs -fno-exceptions -fno-rtti -ffast-math -std=c++11 -frename-registers -fstrict-aliasing -fweb -fomit-frame-pointer
CPP = g++
CPPFLAGS = -fPIC -O3 -mtune=native -march=native -fno-exceptions -fno-rtti -ffast-math -fomit-frame-pointer
LIBS = -lz -lpng -lSDL -s
OBJS = oommf.o vf-tool.o lib-zlib.o files.o text.o vf.o polar.o lib-png.o \
  graphics.o vf-check.o numerical.o tool-thiele-adj-dyn.o filtering.o ovf_diff.o \
  comsol-tool.o fonts-demo.o gen.o tool-thiele-adj.o tool-embed.o oommf_test.o \
  fonts.o vf-debug.o vf-crush.o vf-plot.o vf-view.o tfield_io.o sdlwin.o comsol.o \
  tool-skyfit.o unit.o vf-render.o filepath.o cacheromfs.o tool_any2asciiovf.o \
  fitting.o test_skyrmion_models.o cacheromfs_test.o filepath-test.o object_counter.o \
  tool-include-bundler.o minetest-skins.o test_numerical.o png-diff.o newdisplay.o \
  png_append.o
  

png_append.o: png_append.cpp
	$(CPP) -c $(CPPFLAGS) png_append.cpp

app.o: app.cpp
	$(CPP) -c $(CPPFLAGS)  app.cpp

busybox.elf: busybox.o $(OBJS)
	$(CPP) $(LIBS) -o busybox.elf busybox.o $(OBJS)

object_counter.o: object_counter.cpp
	$(CPP) -c $(CPPFLAGS) object_counter.cpp

png-diff.o: png-diff.cpp
	$(CPP) -c $(CPPFLAGS) png-diff.cpp

tool-thiele-adj-dyn.o: tool-thiele-adj-dyn.cpp tfield_io.hpp fitting.hpp filtering.hpp newdisplay.hpp files.hpp lib-png.hpp filepath.hpp 
	$(CPP) -c $(CPPFLAGS) tool-thiele-adj-dyn.cpp

tool_dmi_borders.o: tool_dmi_borders.cpp
	$(CPP) -c $(CPPFLAGS) tool_dmi_borders.cpp

tool_any2asciiovf.o: tool_any2asciiovf.cpp
	$(CPP) -c $(CPPFLAGS) tool_any2asciiovf.cpp

test_skyrmion_models.o: test_skyrmion_models.cpp skyrmion_models.hpp
	$(CPP) -c $(CPPFLAGS) test_skyrmion_models.cpp

gen.o: gen.cpp
	$(CPP) -c $(CPPFLAGS) gen.cpp

minetest-skins.o: minetest-skins.cpp
	$(CPP) -c $(CPPFLAGS) minetest-skins.cpp

test_numerical.o: test_numerical.cpp
	$(CPP) -c $(CPPFLAGS) test_numerical.cpp
	
tool-embed.o: tool-embed.cpp
	$(CPP) -c $(CPPFLAGS) tool-embed.cpp

ovf_diff.o: ovf_diff.cpp
	$(CPP) -c $(CPPFLAGS) ovf_diff.cpp

tfield_io.o: tfield_io.cpp
	$(CPP) -c $(CPPFLAGS) tfield_io.cpp

oommf_test.o: oommf_test.cpp
	$(CPP) -c $(CPPFLAGS) oommf_test.cpp

busybox.o: busybox.cpp
	$(CPP) -c $(CPPFLAGS) busybox.cpp

vf-render.o: vf-render.cpp
	$(CPP) -c $(CPPFLAGS) vf-render.cpp
	
cacheromfs.o: cacheromfs.cpp cacheromfs.hpp
	$(CPP) -c $(CPPFLAGS) cacheromfs.cpp

cacheromfs_test.o: cacheromfs_test.cpp cacheromfs.hpp
	$(CPP) -c $(CPPFLAGS) cacheromfs_test.cpp
	
filepath-test.o: filepath-test.cpp filepath.hpp
	$(CPP) -c $(CPPFLAGS) filepath-test.cpp

fonts.o: fonts.cpp
	$(CPP) -c $(CPPFLAGS) fonts.cpp

unit.o: unit.cpp unit.hpp
	$(CPP) -c $(CPPFLAGS) unit.cpp

tool-skyfit.o: tool-skyfit.cpp
	$(CPP) -c $(CPPFLAGS) tool-skyfit.cpp

fonts-demo.o: fonts-demo.cpp
	$(CPP) -c $(CPPFLAGS) fonts-demo.cpp

tool-thiele.o: tool-thiele.cpp
	$(CPP) -c $(CPPFLAGS) tool-thiele.cpp

tool-thiele-adj.o: tool-thiele-adj.cpp fitting.hpp numerical.hpp skyrmion_models.hpp
	$(CPP) -c $(CPPFLAGS) tool-thiele-adj.cpp

vf-view.o: vf-view.cpp
	$(CPP) -c $(CPPFLAGS) vf-view.cpp

vf-plot.o: vf-plot.cpp
	$(CPP) -c $(CPPFLAGS) vf-plot.cpp

vf-crush.o: vf-crush.cpp
	$(CPP) -c $(CPPFLAGS) vf-crush.cpp

vf-debug.o: vf-debug.cpp
	$(CPP) -c $(CPPFLAGS) vf-debug.cpp

vf-tool.o: vf-tool.cpp
	$(CPP) -c $(CPPFLAGS) vf-tool.cpp

vf-check.o: vf-check.cpp
	$(CPP) -c $(CPPFLAGS) vf-check.cpp

oommf.o: oommf.cpp oommf.hpp
	$(CPP) -c $(CPPFLAGS) oommf.cpp
	
comsol.o: comsol.cpp comsol.hpp
	$(CPP) -c $(CPPFLAGS) comsol.cpp

tfield.o: tfield.cpp tfield.hpp
	$(CPP) -c $(CPPFLAGS) tfield.cpp

comsol-tool.o: comsol-tool.cpp 
	$(CPP) -c $(CPPFLAGS) comsol-tool.cpp

lib-zlib.o: lib-zlib.cpp lib-zlib.hpp
	$(CPP) -c $(CPPFLAGS) lib-zlib.cpp

files.o: files.cpp files.hpp
	$(CPP) -c $(CPPFLAGS) files.cpp
	
filepath.o: filepath.cpp
	$(CPP) -c $(CPPFLAGS) filepath.cpp

fitting.o: fitting.cpp fitting.hpp fitting_templates.hpp
	$(CPP) -c $(CPPFLAGS) fitting.cpp
	
filtering.o: filtering.cpp filtering.hpp filtering-template.hpp
	$(CPP) -c $(CPPFLAGS) filtering.cpp

text.o: text.cpp text.hpp
	$(CPP) -c $(CPPFLAGS) text.cpp

vf.o: vf.cpp polar64.hpp polar.hpp
	$(CPP) -c $(CPPFLAGS) vf.cpp

polar.o: polar.cpp polar.hpp polar64.hpp
	$(CPP) -c $(CPPFLAGS) polar.cpp

lib-png.o: lib-png.cpp lib-png.hpp
	$(CPP) -c $(CPPFLAGS) lib-png.cpp

graphics.o: graphics.cpp graphics.hpp
	$(CPP) -c $(CPPFLAGS) graphics.cpp

newdisplay.o: newdisplay.cpp newdisplay.hpp graphics.hpp
	$(CPP) -c $(CPPFLAGS) newdisplay.cpp

numerical.o: numerical.cpp
	$(CPP) -c $(CPPFLAGS) numerical.cpp

sdlwin.o: sdlwin.cpp sdlwin.hpp
	$(CPP) -c $(CPPFLAGS) sdlwin.cpp

rcl-all.hpp:
	@exec ./rcl.sh
	
rcl.hpp: tool-include-bundler.elf rcl-all.hpp
	@exec ./tool-include-bundler.elf rcl-all.hpp > rcl.hpp
	
rcl.hpp.gch: rcl.hpp
	$(CPP) -c $(CPPFLAGS) rcl.hpp

tool-include-bundler-sa.o:
	$(CPP) -c $(CPPFLAGS) tool-include-bundler.cpp -D STANDALONE_TOOL -o tool-include-bundler-sa.o

tool-include-bundler.o:
	$(CPP) -c $(CPPFLAGS) tool-include-bundler.cpp

tool-include-bundler.elf: tool-include-bundler-sa.o 
	$(CPP) -s -o tool-include-bundler.elf tool-include-bundler-sa.o

clean : clean_headers clean_objects clean_executables clean_bulk
	@rm -vf *.png

clean_bulk:
	@rm -vf bulk.cpp

clean_objects:
	@rm -vf *.o

clean_headers:
	@rm -vf *.gch
	@rm -vf rcl-all.hpp
	@rm -vf rcl.hpp

clean_executables:
	@rm -vf *.elf
	@rm -vf *.so
