CC = gcc

#OPT = -O3
CFLAGS = -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror
CFLAGS += -Wmissing-declarations

LIBS = -fopenmp

DEBUGFLAGS = -g
DEBUGFLAGS += -ggdb

ASANFLAGS = -fsanitize=address

PAR_SRC = src/parallel.c
SUC_SRC = src/successive.c
DIR = $(shell basename $(shell pwd))

AVG = graphs/AvgTime.png
ACC = graphs/Acceleration.png
EFF = graphs/Efficiency.png

GIT = github.com/Sarkoxed/ParProg2022/blob/master/$(DIR)/

all: parallel.out successive.out $(DIR).md

.PHONY: parallel
parallel: parallel.out

.PHONY: successive
successive: successive.out

.PHONY: report
report: $(DIR).md

.PHONY: graphs
graphs: parallel.out ../tools/graph.py
	-@mkdir graphs
	@printf "t_succ = " > graphs/timing.py
	@./successive.out >> graphs/timing.py
	@printf "\nts = " >> graphs/timing.py
	@./parallel.out >> graphs/timing.py
	@ln ../tools/graph.py graphs/graph.py
	@python graphs/graph.py

parallel.out: $(PAR_SRC)
	@echo Building parallel
	@$(CC) $(CFLAGS) $(ASANFLAGS) $^ -o $@ $(LIBS)

successive.out: $(SUC_SRC)
	@echo Building successive
	@$(CC) $(CFLAGS) $(ASANFLAGS) $^ -o $@ $(LIBS)

$(DIR).md: $(PAR_SRC) $(SUC_SRC) $(AVG) $(ACC) $(EFF)
	echo $(DIR)
	@echo "Creating report"
	@printf "# НИЯУ МИФИ. Лабораторная работа №%s. Соколов Александр, Б20-505. 2022.\n" "$(shell (basename $(shell pwd) | sed 's/[^[:digit:]]//g'))" > $@
	@printf "## Используемая система\n<pre>" >> $@
	@inxi -Cx >> $@
	@printf "</pre>\n" >> $@
	@printf "## Среда разработки\n### Версия OpenMP\n<ul>\n\t<li><b>gcc</b>: 201511</li>\n\t<li><b>clang</b>:201811</li>\n</ul>\n" >> $@
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
	@printf "<code>#pragma lol</code>\n" >> $@
	@printf "## Экспериментальные вычисления\n" >> $@
	@printf "<ul>\n\t<li>Последовательный алгоритм:</li>\n</ul>\n" >> $@
	@printf "\n$(GIT)$(AVG)\n\n" >> $@
	@printf "\n$(GIT)$(ACC)\n\n" >> $@
	@printf "\n$(GIT)$(EFF)\n\n" >> $@
	@printf "## Заключение\n" >> $@
	@printf "## Приложение\n" >> $@
	@printf "### Оценка работы последовательной программы\n" >> $@
	@printf "\n\`\`\`c" >> $@
	@cat $(SUC_SRC) >> $@
	@printf "\n\`\`\`\n" >> $@
	@printf "### Оценка работы параллельной программы\n" >> $@
	@printf "\n\`\`\`c" >> $@
	@cat $(PAR_SRC) >> $@
	@printf "\n\`\`\`\n" >> $@

.PHONY: clean
clean:
	@echo Deleting *.out
	-@rm *.out
