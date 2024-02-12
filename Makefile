python_executable?=python3
environment_name=.venv
py=$(environment_name)/bin/$(python_executable)
GPP?=g++
CFLAGS?=-std=c++11
symbol?=SBIN
strategy?=DMA
start_date?=02/02/2024
end_date?=06/02/2024
x?=2
n?=2
p?=1


.PHONY: all
all: $(environment_name)/pyvenv.cfg main.exe
	$(py) data_processor.py $(symbol) $(start_date) $(end_date) $(n)
	@mv $(symbol).csv history.csv 
	./main.exe $(strategy) $(n) $(x) $(p)
	# @rm -f history.csv

.PHONY: pip_venv
pip_venv: $(environment_name)/pyvenv.cfg
	
main.exe: build/util/csv_parser.o build/strategies.o build/main.o
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