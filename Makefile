python_executable?=python3
environment_name=.venv
py=$(environment_name)/bin/$(python_executable)
GPP?=g++
CFLAGS?=-std=c++11 -g
symbol?=SBIN
strategy?=DMA
start_date?=01/01/2021
end_date?=31/12/2022
x?=5
n?=7


.PHONY: all
all: $(environment_name)/pyvenv.cfg main.exe
	$(py) data_processor.py $(symbol) $(start_date) $(end_date) $(n)
	@mv $(symbol).csv history.csv 
	./main.exe $(strategy) $(n) $(x) $(p) \
			$(max_hold_days) $(c1) $(c2) \
			$(oversold_threshold) $(overbought_threshold) \
			$(adx_threshold) \
			$(threshold) \
			$(stop_loss_threshold)
	# @rm -f history.csv

.PHONY: debug
debug: a.out

.PHONY: pip_venv
pip_venv: $(environment_name)/pyvenv.cfg
	
main.exe: build/util/csv_parser.o build/strategies.o build/main.o build/util/handlers.o
	$(GPP) $(CFLAGS) -o $@ $^

a.out: build/util/csv_parser.o build/strategies.o build/main.o build/util/handlers.o
	$(GPP) $(CFLAGS) -o $@ $^

build/main.o: src/main.cpp
	@mkdir -p build
	$(GPP) $(CFLAGS) -c -o $@ $<

build/strategies.o: $(wildcard src/strategies/*) build/util/csv_parser.o
	@mkdir -p build
	$(GPP) $(CFLAGS) -c -o $@ src/strategies/lib.cpp

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
	@rm -f main.exe daily_cashflow.csv history.csv order_statistics.csv a.out final_pnl.txt
	@rm -rf build