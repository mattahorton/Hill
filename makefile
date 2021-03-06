CXX=g++
INCLUDES=-IRtAudio/ -Ix-api/ -Iy-api -Icore/ -I./ -Ifreetype2

FLAGS=-D__MACOSX_CORE__ $(INCLUDES) -c -std=c++11
LIBS=-framework CoreAudio -framework CoreMIDI -framework CoreFoundation \
	-framework IOKit -framework Carbon  -framework OpenGL \
	-framework GLUT -framework Foundation \
	-framework AppKit -lstdc++ -lm -lfluidsynth -lFTGL -lsndfile


OBJS=   RtAudio/RtAudio.o Hill.o color.o x-api/x-audio.o \
	x-api/x-buffer.o x-api/x-fun.o x-api/x-gfx.o x-api/x-loadlum.o \
	x-api/x-loadrgb.o x-api/x-thread.o x-api/x-vector3d.o y-api/y-charting.o \
	y-api/y-entity.o y-api/y-fft.o y-api/y-fluidsynth.o \
	y-api/y-particle.o y-api/y-waveform.o core/globals.o \
	core/bk-sim.o Mediator.o core/audio.o core/ScoreParser.o \
	sndsrc.o Sukothai.o

Hill: $(OBJS)
	$(CXX) -o Hill $(OBJS) $(LIBS)

Hill.o: Hill.cpp RtAudio/RtAudio.h y-api/y-entity.h x-api/x-vector3d.h
	$(CXX) $(FLAGS) Hill.cpp

RtAudio/RtAudio.o: RtAudio/RtAudio.h RtAudio/RtAudio.cpp RtAudio/RtError.h
	$(CXX) -o RtAudio/RtAudio.o $(FLAGS) RtAudio/RtAudio.cpp

color.o: color.h color.c
	$(CXX) $(FLAGS) color.c

x-api/x-audio.o: x-api/x-audio.h x-api/x-audio.cpp
	$(CXX) -o x-api/x-audio.o $(FLAGS) x-api/x-audio.cpp

x-api/x-buffer.o: x-api/x-buffer.h x-api/x-buffer.cpp
	$(CXX) -o x-api/x-buffer.o $(FLAGS) x-api/x-buffer.cpp

x-api/x-fun.o: x-api/x-fun.h x-api/x-fun.cpp
	$(CXX) -o x-api/x-fun.o $(FLAGS) x-api/x-fun.cpp

x-api/x-gfx.o: x-api/x-gfx.h x-api/x-gfx.cpp
	$(CXX) -o x-api/x-gfx.o $(FLAGS) x-api/x-gfx.cpp

x-api/x-loadlum.o: x-api/x-loadlum.h x-api/x-loadlum.cpp
	$(CXX) -o x-api/x-loadlum.o $(FLAGS) x-api/x-loadlum.cpp

x-api/x-loadrgb.o: x-api/x-loadrgb.h x-api/x-loadrgb.cpp
	$(CXX) -o x-api/x-loadrgb.o $(FLAGS) x-api/x-loadrgb.cpp

x-api/x-thread.o: x-api/x-thread.h x-api/x-thread.cpp
	$(CXX) -o x-api/x-thread.o $(FLAGS) x-api/x-thread.cpp

x-api/x-vector3d.o: x-api/x-vector3d.h x-api/x-vector3d.cpp
	$(CXX) -o x-api/x-vector3d.o $(FLAGS) x-api/x-vector3d.cpp

y-api/y-charting.o: y-api/y-charting.h y-api/y-charting.cpp
	$(CXX) -o y-api/y-charting.o $(FLAGS) y-api/y-charting.cpp

y-api/y-entity.o: y-api/y-entity.h y-api/y-entity.cpp
	$(CXX) -o y-api/y-entity.o $(FLAGS) y-api/y-entity.cpp

y-api/y-fft.o: y-api/y-fft.h y-api/y-fft.cpp
	$(CXX) -o y-api/y-fft.o $(FLAGS) y-api/y-fft.cpp

y-api/y-fluidsynth.o: y-api/y-fluidsynth.h y-api/y-fluidsynth.cpp
	$(CXX) -o y-api/y-fluidsynth.o $(FLAGS) y-api/y-fluidsynth.cpp

y-api/y-particle.o: y-api/y-particle.h y-api/y-particle.cpp
	$(CXX) -o y-api/y-particle.o $(FLAGS) y-api/y-particle.cpp

y-api/y-waveform.o: y-api/y-waveform.h y-api/y-waveform.cpp
	$(CXX) -o y-api/y-waveform.o $(FLAGS) y-api/y-waveform.cpp

core/bk-sim.o: core/bk-sim.h core/bk-sim.cpp
	$(CXX) -o core/bk-sim.o $(FLAGS) core/bk-sim.cpp

core/globals.o: core/globals.h core/globals.cpp
	$(CXX) -o core/globals.o $(FLAGS) core/globals.cpp

Mediator.o: Mediator.h Mediator.cpp
	$(CXX) $(FLAGS) Mediator.cpp

Sukothai.0: Sukothai.h Sukothai.cpp
	$(CXX) $(FLAGS) Sukothai.cpp

core/audio.o: core/audio.h core/audio.cpp
	$(CXX) -o core/audio.o $(FLAGS) core/audio.cpp

core/ScoreParser.o: core/ScoreParser.h core/ScoreParser.cpp
	$(CXX) -o core/ScoreParser.o $(FLAGS) core/ScoreParser.cpp

sndsrc.o: sndsrc.h sndsrc.cpp
	$(CXX) $(FLAGS) sndsrc.cpp

clean:
	rm -f *~ *# *.o */*.o Hill
