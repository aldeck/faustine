TYPE = APP

NAME = Faustine

SRCS = App.cpp

# audio
SRCS += audio/GenericAudioFilterNode.cpp
SRCS += audio/SoundUtils.cpp

# dsp
#SRCS += FirDSP.cpp
#SRCS += KarplusDSP.cpp
	
# ui
SRCS += ui/UIWindow.cpp
SRCS += ui/UIView.cpp
SRCS += ui/PeakView.cpp

LIBS = be media stdc++

RDEFS = Faustine.rdef

## include the makefile-engine
include $(BUILDHOME)/etc/makefile-engine
