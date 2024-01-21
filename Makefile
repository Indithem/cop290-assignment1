python_executable=python3
environment_name=.venv
py=$(environment_name)/bin/python3
cleanup_formats=.csv .json .bin .xlsx .html .tex .xml .feather .parquet .orc .dta .hdf .pkl 
SYMBOL?=BPCL
num_years?=5

.PHONY: all
all: $(environment_name)/pyvenv.cfg
	$(py) data_processor.py $(SYMBOL) $(num_years)

nifty_list.csv:
	wget "https://drive.google.com/uc?export=download&id=15ZAkp5fo7bd7VniWNZQRIhU6LhPLR2kH" -O "nifty_list.csv"

$(environment_name)/pyvenv.cfg: pip_requirements.txt
# 	works for apt in Debian of gradescope servers, dont know about other OS's
	$(python_executable) -m venv $(environment_name)
	@if [$$? -ne 0]; then\
		sudo apt install python3-venv;\
		$(python_executable) -m venv $(environment_name);\
	fi 
	$(py) -m pip install -r $<

.PHONY: clean
clean:
	# @rm -rf $(environment_name)
	@rm -f nifty_list.csv
	@rm -rf __pycache__
	@rm -f $(foreach ff, $(cleanup_formats), *$(ff))
	@find . -name '*.txt' ! -name 'pip_requirements.txt' -exec rm {} +