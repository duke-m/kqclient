### FreeBSD-Makefile ###

MAINTAINER=	Mario Duhanic

USE_GITHUB=	yes
GH_ACCOUNT=	duke-m

OBJS	= kqclient.o

PRG		= kqclient

CFLAGS = -O2 -Weverything

#have a little nap
NAP = sleep 0.5

#don't buffer the output:
NO_BUF = stdbuf -o0
UUID != uuidgen

#tests:
PWD != pwd -P
T_WRITE = $(PWD)/$(UUID) WRITE $(UUID) (14)
T_RENAME = $(PWD)/$(UUID) RENAME $(UUID) (32)
T_DELETE = $(PWD)/$(UUID) DELETE $(UUID) (1)

all: $(PRG) test

$(PRG)	: $(OBJS)
	$(CC) $(CFLAGS) -o $(.TARGET) $(.ALLSRC)


$(OBJS)	: $(.PREFIX).c
	$(CC) $(CFLAGS) -c $(.PREFIX).c

clean:
	rm $(PRG) $(OBJS)

test:
	@echo "$(T_WRITE)" > $(UUID).compare
	@echo "$(T_RENAME)" >> $(UUID).compare 
	@echo "$(T_DELETE)" >> $(UUID).compare
	@touch $(UUID)
	@$(NO_BUF) ./$(PRG) $(UUID) >> $(UUID).out & echo $$! > $(UUID).pid
	@$(NAP)
	@echo 123 > $(UUID)
	@mv $(UUID) $(UUID)-moved
	@rm $(UUID)-moved
	@$(NAP)
	@kill `cat $(UUID).pid`

	@printf %s "TESTS: "
	@diff -qas $(UUID).compare $(UUID).out | sed 's/Files.* are identical/Passed./' | sed 's/Files.* differ/FAILED!/'

	@rm $(UUID).*