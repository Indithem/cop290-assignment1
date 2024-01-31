# Trading Simulator and Analyzer

## What this does?

## How to use?
### To use
* Clone this repository.
* Run `make`. The server should be started.

### To clean
* Run `make clean`

## Known issues
* pip virtual environment might not get properly installed. 
    Only a temporary workaround is provided for debian based systems.(On which the gradescope servers run)

## Example


## Further plans
* As per the assignment, UI for the following features
    * Plot graph in defined time scale.
    * Plot multiple stocks in same time scale.
    * Apply technical filters.
* Login System
    * Confirm password
    * Forgot and Change password 
    * Give registration option if not registered
    * Validity of username in registration page before submit button is clicked
* Filters
    * p/e ratio
    * average (VWAP=average)
    * sort high to low/low to high/ alphabetical  
* Cache downloaded data(if needed, I think yfinance caches downloads)
* hourly data
* Ads
* Download today's data for all available stocks
* Update today's data for a particular stock if the data downloaded before is outdated
* Stocks being updated live before graph is plotted as in tradingview.com 
* Progressive Web App