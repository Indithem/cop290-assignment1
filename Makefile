python_executable=python3
environment_name=.venv
py=$(environment_name)/bin/python3
cleanup_formats=.csv .json .bin .xlsx .html .tex .xml .feather .parquet .orc .dta .hdf .pkl .png .bson .yaml
symbol?=BPCL
strategy?=BASIC
start_date?=01/01/2021
end_date?=31/12/2021


.PHONY: all
all: $(environment_name)/pyvenv.cfg main.exe
	$(py) data_processor.py $(symbol) $(start_date) $(end_date)
	@mv $(symbol).csv history.csv 
	./main.exe $(strategy) $(n) $(x) 
	@rm -f history.csv

.PHONY: pip_venv
pip_venv: $(environment_name)/pyvenv.cfg
	
main.exe: src/
	g++ -std=c++11 -o $@ src/main.cpp 

$(environment_name)/pyvenv.cfg: pip_requirements.txt
# 	works for apt in Debian of gradescope servers, dont know about other OS's
	$(python_executable) -m venv $(environment_name) --quiet || python3 -m venv $(environment_name) --without-pip --system-site-packages --quiet
	$(py) -m pip install -r $<

.PHONY: clean
clean:
	@rm -rf $(environment_name)
	@rm -f nifty_list.csv
	@rm -rf __pycache__
	@rm -f $(foreach ff, $(cleanup_formats), *$(ff))
	@find . -name '*.txt' ! -name 'pip_requirements.txt' -exec rm {} +