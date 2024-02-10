python_executable=python3
environment_name=.venv
py=$(environment_name)/bin/python3
strategies_cpps = adx basic dma_2 dma macd rsi
symbol?=SBIN
strategy?=BASIC
start_date?=01/01/2021
end_date?=31/12/2022
x?=5
n?=7


.PHONY: all
all: $(environment_name)/pyvenv.cfg main.exe
	$(py) data_processor.py $(symbol) $(start_date) $(end_date) $(n)
	@mv $(symbol).csv history.csv 
	./main.exe $(strategy) $(n) $(x) $(p)
	# @rm -f history.csv

.PHONY: pip_venv
pip_venv: $(environment_name)/pyvenv.cfg
	
main.exe: build/strategies.o
	g++ -std=c++11 -c -o build/main.o src/main.cpp 
	g++ -std=c++11 -o $@ build/main.o build/strategies.o

build/strategies.o: $(foreach ff, $(strategies_cpps), src/strategies/$(ff).cpp)
	@mkdir -p build
	g++ -std=c++11 -c -o $@ src/strategies/lib.cpp

$(environment_name)/pyvenv.cfg: pip_requirements.txt
# 	works for apt in Debian of gradescope servers, dont know about other OS's
	$(python_executable) -m venv $(environment_name) || python3 -m venv $(environment_name) --without-pip --system-site-packages
	$(py) -m pip install -r $< --quiet

.PHONY: clean
clean:
	@rm -rf $(environment_name)
	@rm -f nifty_list.csv
	@rm -rf __pycache__
	@rm -f $(foreach ff, $(cleanup_formats), *$(ff))
	@rm -f main.exe daily_cashflow.csv history.csv order_statistics.csv a.out final_pnl.txt
	@rm -rf build