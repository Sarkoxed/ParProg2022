CC = gcc

#OPT = -O3
CFLAGS = -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
#CFLAGS += -Werror
CFLAGS += -Wmissing-declarations

LIBS = -fopenmp

DEBUGFLAGS = -g
DEBUGFLAGS += -ggdb

ASANFLAGS = -fsanitize=address

PAR_SRC = src/parallel.c
DIR = $(shell basename $(shell pwd))

AVG = graphs/AvgTime
ACC = graphs/Acceleration
EFF = graphs/Efficiency

GIT = github.com/Sarkoxed/ParProg2022/blob/master/$(DIR)/

all: parallel.out $(DIR).md

.PHONY: parallel
parallel: parallel.out

.PHONY: report
report: $(DIR).md

.PHONY: graphs
graphs: parallel.out graphs/graph.py 
	@echo "Static"
	@cat src/parallel.c | grep schedule
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)static_1.png $(ACC)static_1.png $(EFF)static_1.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.static./schedule(static, 1)/' src/parallel.c
	@echo "Static1"
	@cat src/parallel.c | grep schedule
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)static_2.png $(ACC)static_2.png $(EFF)static_2.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.static. 1./schedule(static, 2)/' src/parallel.c
	@echo "Static2"
	@cat src/parallel.c | grep schedule
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)static_3.png $(ACC)static_3.png $(EFF)static_3.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.static. 2./schedule(dynamic)/' src/parallel.c
	@echo "Dynamic"
	@cat src/parallel.c | grep schedule
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)dynamic_1.png $(ACC)dynamic_1.png $(EFF)dynamic_1.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.dynamic./schedule(dynamic, 2)/' src/parallel.c
	@echo "Dynamic2"
	@cat src/parallel.c | grep schedule
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)dynamic_2.png $(ACC)dynamic_2.png $(EFF)dynamic_2.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.dynamic. 2./schedule(guided)/' src/parallel.c
	@echo "Guieded 1"
	@cat src/parallel.c | grep schedule
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)guided_1.png $(ACC)guided_1.png $(EFF)guided_1.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.guided./schedule(guided, 2)/' src/parallel.c
	@echo "Guieded 2"
	@cat src/parallel.c | grep schedule
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)guided_2.png $(ACC)guided_2.png $(EFF)guided_2.png
	@sed -i 's/schedule.guided. 2./schedule(static)/' src/parallel.c
	@cat src/parallel.c | grep schedule

%.out: src/%.c
	@echo Building $@
	@$(CC) $(CFLAGS) $(ASANFLAGS) $^ -o $@ $(LIBS)

chars.out: src/omp_funcs.c
	@echo Building Funcs
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(DIR).md: graphs $(PAR_SRC) chars.out src/lock.c
	echo $(DIR)
	@echo "Creating report"
	@printf "# НИЯУ МИФИ. Лабораторная работа №%s. Соколов Александр, Б20-505. 2022.\n" "$(shell (basename $(shell pwd) | sed 's/[^[:digit:]]//g'))" > $@
	@printf "## Среда разработки\n<pre>" >> $@
	@sudo inxi -SCmxz >> $@
	@printf "OpenMP:\n\tgcc: 201511\n" >> $@
	@printf "</pre>\n" >> $@
	@printf "\n\`\`\`c\n" >> $@
	@./chars.out >> $@
	@printf "\n\`\`\`\n" >> $@
	@printf "## Значение директив\n" >> $@
	@printf "<code>" >> $@
	@cat src/parallel.c | grep "pragma omp" >> $@
	@printf "</code>\n" >> $@
	@printf "\n\`\`\`c\n" >> $@
	@cat src/lock.c >> $@
	@printf "\n\`\`\`\n" >> $@
	@printf "## Экспериментальные вычисления\n" >> $@
	@printf "\nStatic" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG)static_1.png)\n\n" >> $@
	@printf "\nStatic1" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG)static_2.png)\n\n" >> $@
	@printf "\nStatic2" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG)static_3.png)\n\n" >> $@
	@printf "\nDynamic" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG)dynamic_1.png)\n\n" >> $@
	@printf "\nDynamic2" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG)dynamic_2.png)\n\n" >> $@
	@printf "\nGuieded1" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG)guided_1.png)\n\n" >> $@
	@printf "\nGuided2" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG)guided_2.png)\n\n" >> $@
	@printf "## Заключение\n" >> $@
	@printf "## Приложение\n" >> $@
	@printf "### Оценка работы параллельной программы\n" >> $@
	@printf "\n\`\`\`c\n" >> $@
	@cat src/omp_funcs.c >> $@
	@printf "\n\`\`\`\n" >> $@
	@printf "\n\`\`\`c\n" >> $@
	@cat $(PAR_SRC) >> $@
	@printf "\n\`\`\`\n" >> $@

.PHONY: clean
clean:
	@echo Deleting *.out
	@rm *.out 2>/dev/null || true
