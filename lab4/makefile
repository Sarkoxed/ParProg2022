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
SUC_SRC = src/successive.c
DIR = $(shell basename $(shell pwd))

AVG = graphs/AvgTime
ACC = graphs/Acceleration
EFF = graphs/Efficiency

GIT = github.com/Sarkoxed/ParProg2022/blob/master/$(DIR)/

all: parallel.out successive.out $(DIR).md

.PHONY: check
check: parallel_check.out
	@echo "Checking that sort works fine"
	@./parallel_check.out > check/arrs.py && python check/check.py 10

.PHONY: parallel
parallel: parallel.out

.PHONY: successive
successive: successive.out

.PHONY: report
report: $(DIR).md

.PHONY: graphs
graphs: parallel.out graphs/graph.py 
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)static_1.png $(ACC)static_1.png $(EFF)static_1.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.static./schedule(static, 1)/' src/parallel.c
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)static_2.png $(ACC)static_2.png $(EFF)static_2.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.static. 1./schedule(static, 2)/' src/parallel.c
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)static_3.png $(ACC)static_3.png $(EFF)static_3.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.static. 2./schedule(dynamic)/' src/parallel.c
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)dynamic_1.png $(ACC)dynamic_1.png $(EFF)dynamic_1.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.dynamic./schedule(dynamic, 2)/' src/parallel.c
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)dynamic_2.png $(ACC)dynamic_2.png $(EFF)dynamic_2.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.dynamic. 2./schedule(guided)/' src/parallel.c
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)guided_1.png $(ACC)guided_1.png $(EFF)guided_1.png
	@printf "t_succ = " > graphs/timing.py
	@sed -i 's/schedule.guided./schedule(guided, 2)/' src/parallel.c
	@printf "ts = [" > graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@printf "]\n" >> graphs/timing.py
	@python graphs/graph.py $(AVG)guided_2.png $(ACC)guided_2.png $(EFF)guided_2.png

%.out: src/%.c
	@echo Building $@
	@$(CC) $(CFLAGS) $(ASANFLAGS) $^ -o $@ $(LIBS)

successive.out: $(SUC_SRC)
	@echo Building successive
	@$(CC) $(CFLAGS) $(ASANFLAGS) $^ -o $@ $(LIBS)

$(DIR).md: $(PAR_SRC) $(SUC_SRC) $(AVG) $(ACC) $(EFF)
	echo $(DIR)
	@echo "Creating report"
	@printf "# НИЯУ МИФИ. Лабораторная работа №%s. Соколов Александр, Б20-505. 2022.\n" "$(shell (basename $(shell pwd) | sed 's/[^[:digit:]]//g'))" > $@
	@printf "## Среда разработки\n<pre>" >> $@
	@sudo inxi -SCmxz >> $@
	@printf "OpenMP:\n\tgcc: 201511\n" >> $@
	@printf "</pre>\n" >> $@
	@printf "\n## Временная оценка алгоритма\n\n" >> $@
	
	@printf "## Анализ алгоритма\n#### Принцип работы\n\n" >> $@
	@printf "#### Блок-схема\n\n\`\`\`mermaid\n\t" >> $@
	@echo "%%{ init : { \"flowchart\" : { \"curve\" : \"stepAfter\" }}}%%" >> $@
	@printf "\tgraph LR\n" >> $@
	@printf "\t\tA --> B(text)\n" >> $@
	@printf "\t\tB --> |Да| D[\"max = array[i]\"]\n" >> $@
	@printf "\t\tB --> E(выход)\n" >> $@
	@printf "\`\`\`\n\n" >> $@
	@printf "## Значение директив\n" >> $@
	@printf "<code>" >> $@
	@cat src/parallel.c | grep "pragma omp" >> $@
	@printf "</code>\n" >> $@
	@printf "## Экспериментальные вычисления\n" >> $@
	@printf "<ul>\n\t<li>Последовательный алгоритм:</li>\n</ul>\n" >> $@
	@printf "\n![AvgTime](https://$(GIT)$(AVG))\n\n" >> $@
	@printf "\n![Acceleration](https://$(GIT)$(ACC))\n\n" >> $@
	@printf "\n![Efficiency](https://$(GIT)$(EFF))\n\n" >> $@
	@printf "## Заключение\n" >> $@
	@printf "## Приложение\n" >> $@
	@printf "### Оценка работы последовательной программы\n" >> $@
	@printf "\n\`\`\`c\n" >> $@
	@cat $(SUC_SRC) >> $@
	@printf "\n\`\`\`\n" >> $@
	@printf "### Оценка работы параллельной программы\n" >> $@
	@printf "\n\`\`\`c\n" >> $@
	@cat $(PAR_SRC) >> $@
	@printf "\n\`\`\`\n" >> $@

.PHONY: clean
clean:
	@echo Deleting *.out
	@rm *.out 2>/dev/null || true
