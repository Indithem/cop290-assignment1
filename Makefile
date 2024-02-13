python_executable?=python3
environment_name=.venv
py=$(environment_name)/bin/$(python_executable)
GPP?=g++
CFLAGS?=-std=c++11 -g 
symbol?=SBIN
symbol1?=$(symbol)
strategy?=DMA
start_date?=27/02/2023
end_date?=06/02/2024
x?=3

ifeq ($(strategy),BEST_OF_ALL)
	train_start_date := $(shell date -d "$(shell echo $(start_date) | awk -F'/' '{print $$2"/"$$1"/"$$3}') - 1 year" +%d/%m/%Y)
	train_end_date := $(shell date -d "$(shell echo $(end_date) | awk -F'/' '{print $$2"/"$$1"/"$$3}') - 1 year" +%d/%m/%Y)
	n=50
endif

n?=0

.PHONY: all
all: $(environment_name)/pyvenv.cfg main.exe
	$(py) data_processor.py $(symbol1) $(start_date) $(end_date) $(n) $(symbol2) $(train_start_date) $(train_end_date)
	./main.exe $(strategy) n=$(n) x=$(x) p=$(p) \
			max_hold_days=$(max_hold_days) c1=$(c1) c2=$(c2) \
			oversold_threshold=$(oversold_threshold) overbought_threshold=$(overbought_threshold) \
			adx_threshold=$(adx_threshold) \
			threshold=$(threshold) \
			stop_loss_threshold=$(stop_loss_threshold)
	# @rm -f history.csv history2.csv

.PHONY: debug
debug: main.exe
	@mv main.exe a.out

.PHONY: pip_venv
pip_venv: $(environment_name)/pyvenv.cfg
	
main.exe: build/util/matrices.o build/util/csv_parser.o build/strategies.o build/handlers.o build/main.o
	$(GPP) $(CFLAGS) -o $@ $^

build/main.o: src/main.cpp
	@mkdir -p build
	$(GPP) $(CFLAGS) -c -o $@ $<

build/strategies.o: $(wildcard src/strategies/*)
	@mkdir -p build
	$(GPP) $(CFLAGS) -c -o $@ src/strategies/lib.cpp

build/handlers.o: $(wildcard src/handlers/*)
	@mkdir -p build
	$(GPP) $(CFLAGS) -c -o $@ src/handlers/handlers.cpp

build/util/%.o: src/util/%.cpp src/util/%.h
	@mkdir -p build/util
	$(GPP) $(CFLAGS) -c -o $@ $<


$(environment_name)/pyvenv.cfg: pip_requirements.txt
# 	works for apt in Debian of gradescope servers, dont know about other OS's
	$(python_executable) -m venv $(environment_name) || $(python_executable) -m venv $(environment_name) --without-pip --system-site-packages
	$(py) -m pip install -r $< --quiet

.PHONY: clean
clean:
	@rm -rf $(environment_name)
	@rm -f nifty_list.csv
	@rm -rf __pycache__
	@rm -f $(foreach ff, $(cleanup_formats), *$(ff))
	@rm -f main.exe *.csv a.out final_pnl.txt
	@rm -rf build