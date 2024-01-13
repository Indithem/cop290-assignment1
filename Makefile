python_executable=python
environment_name=venv
py=$(environment_name)/bin/python3
# todo: rename year and symbol as per assignment
inp_year?=2020
inp_symbol?=BPCL

var_year=$(inp_year)
var_symbol=$(inp_symbol)

.PHONY: all
all: $(environment_name) 
	

nifty_list.csv:
	wget "https://drive.google.com/uc?export=download&id=15ZAkp5fo7bd7VniWNZQRIhU6LhPLR2kH" -O "nifty_list.csv"

$(environment_name): pip_requirements.txt
	$(python_executable) -m venv $(environment_name)
	$(py) -m pip install -r $<

.PHONY: clean
clean:
	rm -rf $(environment_name)
	rm nifty_list.csv