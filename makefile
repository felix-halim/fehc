CC			=	g++
CFLAGS		=	-O3 -Wall -I. -Isrc
RM 			=	rm -fr
SRCDIR		=	src
OUTDIR		=	bin
DATADIR		=	data
SHELL		=	/bin/sh
MAKE		=	make  

.IGNORE:  
.SILENT: 

.SUFFIXES:
.SUFFIXES: .cpp .c .o

all: $(OUTDIR)/gdy 		\
     $(OUTDIR)/gdy_dp 	\
     $(OUTDIR)/gdy_bdp 	\
     $(OUTDIR)/ahistl 	\
     $(OUTDIR)/dns 		\
     $(OUTDIR)/vopt 	\
     $(OUTDIR)/mhist 	\
     $(OUTDIR)/maxdiff

TESTER_H_DEP	=	$(SRCDIR)/tester.h $(SRCDIR)/random.h

$(OUTDIR)/gdy: $(SRCDIR)/gdy.cpp $(SRCDIR)/gdy_sol.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/gdy $(SRCDIR)/gdy.cpp

$(OUTDIR)/gdy_dp: $(SRCDIR)/gdy_dp.cpp $(SRCDIR)/gdy_sol.h $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/gdy_dp $(SRCDIR)/gdy_dp.cpp

$(OUTDIR)/gdy_bdp: $(SRCDIR)/gdy_bdp.cpp $(SRCDIR)/gdy_sol.h $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/gdy_bdp $(SRCDIR)/gdy_bdp.cpp

$(OUTDIR)/gdy_ls: $(SRCDIR)/gdy_ls.cpp $(SRCDIR)/gdy_sol.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/gdy_ls $(SRCDIR)/gdy_ls.cpp

$(OUTDIR)/vopt: $(SRCDIR)/vopt.cpp $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/vopt $(SRCDIR)/vopt.cpp

$(OUTDIR)/mhist: $(SRCDIR)/mhist.cpp $(SRCDIR)/gdy_sol.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/mhist $(SRCDIR)/mhist.cpp

$(OUTDIR)/maxdiff: $(SRCDIR)/maxdiff.cpp $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/maxdiff $(SRCDIR)/maxdiff.cpp

$(OUTDIR)/ahistl: $(SRCDIR)/ahistl.cpp $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/ahistl $(SRCDIR)/ahistl.cpp

$(OUTDIR)/dns: $(SRCDIR)/dns.cpp $(SRCDIR)/vopt.h $(TESTER_H_DEP)
	$(CC) $(CFLAGS) -o $(OUTDIR)/dns $(SRCDIR)/dns.cpp


.PHONY : clean

clean:
	$(RM) $(OUTDIR)/*

distclean:
	$(MAKE) clean

again:
	$(MAKE) distclean
