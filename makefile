CC			=	g++
CFLAGS		=	-O3 -Wall -I. -Isrc
SRCDIR		=	src
DATADIR		=	data

.IGNORE:  
.SILENT: 

.SUFFIXES:
.SUFFIXES: .cpp .c .o

all: gdy gdy_dp gdy_bdp ahistl dns vopt mhist maxdiff

TESTER_H_DEP	=	$(SRCDIR)/tester.h $(SRCDIR)/random.h

gdy: $(SRCDIR)/gdy.cpp $(SRCDIR)/gdy_sol.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o gdy $(SRCDIR)/gdy.cpp

gdy_dp: $(SRCDIR)/gdy_dp.cpp $(SRCDIR)/gdy_sol.h $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o gdy_dp $(SRCDIR)/gdy_dp.cpp

gdy_bdp: $(SRCDIR)/gdy_bdp.cpp $(SRCDIR)/gdy_sol.h $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o gdy_bdp $(SRCDIR)/gdy_bdp.cpp

gdy_ls: $(SRCDIR)/gdy_ls.cpp $(SRCDIR)/gdy_sol.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o gdy_ls $(SRCDIR)/gdy_ls.cpp

vopt: $(SRCDIR)/vopt.cpp $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o vopt $(SRCDIR)/vopt.cpp

mhist: $(SRCDIR)/mhist.cpp $(SRCDIR)/gdy_sol.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o mhist $(SRCDIR)/mhist.cpp

maxdiff: $(SRCDIR)/maxdiff.cpp $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o maxdiff $(SRCDIR)/maxdiff.cpp

ahistl: $(SRCDIR)/ahistl.cpp $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o ahistl $(SRCDIR)/ahistl.cpp

dns: $(SRCDIR)/dns.cpp $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o dns $(SRCDIR)/dns.cpp


.PHONY : clean

clean:
	rm gdy gdy_dp gdy_bdp ahistl dns vopt mhist maxdiff
