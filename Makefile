python_executable=python3
environment_name=.venv
environment_name2=.venv2
py=$(environment_name)/bin/python3
py2=$(environment_name2)/bin/python3
SYMBOL?=BPCL
num_years?=5

.PHONY: all
all: $(environment_name2)/pyvenv.cfg
	# @rm -rf instance
	$(py2) -m flask --debug run 

stock_data/nifty_list.csv:
	@wget "https://drive.google.com/uc?export=download&id=15ZAkp5fo7bd7VniWNZQRIhU6LhPLR2kH" -O "stocks/data/nifty_list.csv"

$(environment_name)/pyvenv.cfg: pip_requirements.txt
# 	works for apt in Debian of gradescope servers, dont know about other OS's
	$(python_executable) -m venv $(environment_name) || $(python_executable) -m venv $(environment_name) --without-pip --system-site-packages
	$(py) -m pip install -r $< 

$(environment_name2)/pyvenv.cfg: requirements.txt
# 	works for apt in Debian of gradescope servers, dont know about other OS's
	$(python_executable) -m venv $(environment_name2) || $(python_executable) -m venv $(environment_name2) --without-pip --system-site-packages
	$(py2) -m pip install -r $< 

.PHONY: clean
clean:
	@rm -rf $(environment_name)
	@rm -f nifty_list.csv
	@rm -rf __pycache__
	@rm -rf benchmarks
	@rm -rf stock_data