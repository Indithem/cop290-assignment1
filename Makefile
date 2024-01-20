python_executable=python3
environment_name=.venv
py=$(environment_name)/bin/python3
cleanup_formats=.csv .json .bin .xlsx .html .tex .xml .feather .parquet .orc .dta .hdf .pkl 
SYMBOL?=BPCL
num_years?=5

.PHONY: all
all: $(environment_name)/pyvenv.cfg
	$(py) main.py $(SYMBOL) $(num_years)

nifty_list.csv:
	wget "https://drive.google.com/uc?export=download&id=15ZAkp5fo7bd7VniWNZQRIhU6LhPLR2kH" -O "nifty_list.csv"

$(environment_name)/pyvenv.cfg: pip_requirements.txt
	$(python_executable) -m venv $(environment_name)
	$(py) -m pip install -r $<

.PHONY: clean
clean:
	# @rm -rf $(environment_name)
	@rm -f nifty_list.csv
	@rm -rf __pycache__
	@rm -f $(foreach ff, $(cleanup_formats), *$(ff))
	@mv pip_requirements.txt pip_requirements.txt.bak
	@rm -f *.txt
	@mv pip_requirements.txt.bak pip_requirements.txt