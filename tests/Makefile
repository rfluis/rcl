CPPFLAGS = -fPIC -O3 -mtune=native -march=native -fno-exceptions -fno-rtti -ffast-math -frename-registers -fstrict-aliasing -fweb -fomit-frame-pointer -I../
CCPP = g++ -c $(CPPFLAGS)
LD = g++ -s $(CPPFLAGS)

all: 
	@echo Expecify an objetive:

test-tpoint.o: test-tpoint.cpp
	 $(CCPP) test-tpoint.cpp
	
test-tpoint: test-tpoint.o
	$(LD) test-tpoint.o -o test-tpoint
	./test-tpoint

test-tpoint-math.o: test-tpoint-math.cpp
	$(CCPP) test-tpoint-math.cpp

test-tpoint-math: test-tpoint-math.o 
	$(LD) test-tpoint-math.o -o test-tpoint-math
	./test-tpoint-math

test-skyrmion-models.o: test-skyrmion-models.cpp 
	$(CCPP) test-skyrmion-models.cpp

app_register_1.o: app_register_1.cpp app_register.hpp
	$(CCPP) app_register_1.cpp

app_register_2.o: app_register_2.cpp app_register.hpp
	$(CCPP) app_register_2.cpp

app_register_3.o: app_register_3.cpp app_register.hpp
	$(CCPP) app_register_3.cpp

app_register_4.o: app_register_4.cpp app_register.hpp
	$(CCPP) app_register_4.cpp

app_register.o: app_register.cpp app_register.hpp
	$(CCPP) app_register.cpp

app_register_greg.o: app_register_greg.cpp app_register.hpp
	$(CCPP) app_register_greg.cpp

app_register: app_register_3.o app_register_2.o app_register_4.o app_register.o app_register_1.o app_register_greg.o
	$(LD) app_register_3.o app_register_2.o app_register_4.o app_register.o app_register_1.o app_register_greg.o -o app_register
