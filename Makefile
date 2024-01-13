python_executable=python
environment_name=venv
py=$(environment_name)/bin/python3

$(environment_name): pip_requirements.txt
	$(python_executable) -m venv $(environment_name)
	$(py) -m pip install -r $<

clean:
	rm -rf $(environment_name)