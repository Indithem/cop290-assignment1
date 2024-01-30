import os
import shutil
import pandas
import yfinance as yf
import sys
from enum import Enum
import requests
import timeit
import plotly.graph_objects as go
import plotly
import json
from datetime import datetime, timedelta

DATA_FOLDER = "instance/stocks_data"
FORMATS_FILE = "stocks_data_formats.json"


class Downloader:
    """
    Downloads data as required for the assignment.

    BENCHMARK_REPEATS: Number of times to repeat the benchmarking
    REQUIRED_COLUMNS: Columns required for the assignment, ordered

    All the write_to_file_format functions should be named as `write_to_<format>`
    If the same format has multiple testing methods, then the function name should be `write_to_<format>_<method>`
    The file should be saved as `<SYMBOL>_<method>.<format>`
    This enables automated benchmarking of those functions.
    """

    BENCHMARK_REPEATS = 1

    def __init__(self, SYMBOL: str) -> None:
        self.SYMBOL = SYMBOL
        self.data = self.download_data()

    def download_data(self) -> pandas.DataFrame:
        df: pandas.DataFrame = yf.download(self.SYMBOL, period="10y").reset_index()
        return pandas.DataFrame(
            {
                "Date": df["Date"],
                "Adj Close": df["Adj Close"],
            }
        )

    def benchmark(self) -> list[(str, float, float)]:
        benchmark_results = list[(str, float, float)]()
        if not os.path.exists(DATA_FOLDER):
            os.makedirs(DATA_FOLDER)
        for i, f in enumerate(dir(self)):
            print(f"Progress : {i}/{len(dir(self))}% benchmarking {f}")
            if "write_to" in f and callable(getattr(self, f)):
                splitted = f.removeprefix("write_to_").split("_")
                fileformat = splitted[0]
                test_name = ("_" + splitted[1]) if len(splitted) > 1 else ""
                time = (
                    timeit.Timer(lambda: getattr(self, f)()).timeit(
                        number=self.BENCHMARK_REPEATS
                    )
                    / self.BENCHMARK_REPEATS
                )
                size = os.stat(
                    f"{DATA_FOLDER}/{self.SYMBOL}{test_name}.{fileformat}"
                ).st_size
                benchmark_results.append((f"{fileformat}{test_name}", time, size))
        return benchmark_results

    def write_to_csv(self) -> None:
        self.data.to_csv(f"{DATA_FOLDER}/{self.SYMBOL}.csv", index=False)

    # def write_to_txt_asStr(self) -> None:
    #     with open(f"{DATA_FOLDER}/{self.SYMBOL}_asStr.txt", "w") as f:
    #         f.write(self.data.to_string(index=False))

    def write_to_txt(self) -> None:
        self.data.to_csv(f"{DATA_FOLDER}/{self.SYMBOL}.txt", index=False, sep="\t")

    # def write_to_json_records(self) -> None:
    #     self.data.to_json(f"{DATA_FOLDER}/{self.SYMBOL}_records.json", orient="records")

    # def write_to_json_columns(self) -> None:
    #     self.data.to_json(f"{DATA_FOLDER}/{self.SYMBOL}_columns.json", orient="columns")

    # def write_to_json_split(self) -> None:
    #     self.data.to_json(f"{DATA_FOLDER}/{self.SYMBOL}_split.json", orient="split")

    # def write_to_json_index(self) -> None:
    #     self.data.to_json(f"{DATA_FOLDER}/{self.SYMBOL}_index.json", orient="index")

    def write_to_json(self) -> None:
        # json_split is found to be the best
        self.data.to_json(f"{DATA_FOLDER}/{self.SYMBOL}.json", orient="values")

    # def write_to_json_table(self) -> None:
    #     self.data.to_json(f"{DATA_FOLDER}/{self.SYMBOL}_table.json", orient="table")

    def write_to_xlsx(self) -> None:
        self.data.to_excel(f"{DATA_FOLDER}/{self.SYMBOL}.xlsx", index=False)

    def write_to_html(self) -> None:
        self.data.to_html(f"{DATA_FOLDER}/{self.SYMBOL}.html", index=False)

    def write_to_tex(self) -> None:
        self.data.to_latex(f"{DATA_FOLDER}/{self.SYMBOL}.tex", index=False)

    def write_to_xml(self) -> None:
        self.data.rename(lambda x: x.replace(" ", "_"), axis=1).to_xml(
            f"{DATA_FOLDER}/{self.SYMBOL}.xml", index=False
        )

    def write_to_feather(self) -> None:
        self.data.to_feather(f"{DATA_FOLDER}/{self.SYMBOL}.feather")

    def write_to_parquet(self) -> None:
        self.data.to_parquet(f"{DATA_FOLDER}/{self.SYMBOL}.parquet")

    def write_to_orc(self) -> None:
        self.data.to_orc(f"{DATA_FOLDER}/{self.SYMBOL}.orc")

    def write_to_dta(self) -> None:
        self.data.rename(lambda x: x.replace(" ", "_"), axis=1).to_stata(
            f"{DATA_FOLDER}/{self.SYMBOL}.dta", write_index=False
        )

    def write_to_hdf(self) -> None:
        self.data.to_hdf(f"{DATA_FOLDER}/{self.SYMBOL}.hdf", key="data", mode="w")

    def write_to_pkl(self) -> None:
        self.data.to_pickle(f"{DATA_FOLDER}/{self.SYMBOL}.pkl")

    # def write_to_yaml(self) -> None:
    #     yaml.dump(self.data.to_dict(), open(f"{DATA_FOLDER}/{self.SYMBOL}.yaml", "w"))

    # def write_to_bson(self) -> None:
    #     with open(f"{DATA_FOLDER}/{self.SYMBOL}.bson", "wb") as f:
    #         f.write(bson.dumps(self.data.to_dict()))


class formats(Enum):
    daily = 1
    weekly = 2
    monthly = 3
    quarterly = 4
    yearly = 5


class Saver:
    def __init__(self):
        pass

    def save_graph(self, symbols=None, data: list[pandas.DataFrame] = None):
        symbols = symbols or self.symbols
        data = data or self.ret_data

        fig = go.Figure()
        for i, d in enumerate(data):
            fig.add_trace(
                go.Scatter(
                    x=list(d["Date"]), y=d["Adj Close"], mode="lines", name=symbols[i]
                )
            )

        fig.update_layout(
            xaxis_title="DATE",
            yaxis_title="closing price",
            legend=dict(
                title="Symbols",
                orientation="h",
                yanchor="bottom",
                y=1.02,
                xanchor="right",
                x=1,
            ),
            margin=dict(l=40, r=40, t=40, b=40),
        )
        fig.show()
        plotly.io.write_html(fig, f"{DATA_FOLDER}/graph.html")

    def save_graph_data(self, symbols, format):
        self.symbols = symbols
        self.ret_data = []
        for symbol in symbols:
            if not os.path.exists(f"{DATA_FOLDER}/{symbol}.feather"):
                p = Downloader(symbol)
                data = p.data
                p.write_to_feather()
            else:
                data = pandas.read_feather(f"{DATA_FOLDER}/{symbol}.feather")
            match format:
                case formats.daily:
                    data = data[:10]
                case formats.weekly:
                    data = data[: 7 * 10]
                case formats.monthly:
                    data = data[: 30 * 10]
                case formats.quarterly:
                    data = data[: 90 * 10]
                case formats.yearly:
                    data = data[: 365 * 10]
            self.ret_data.append(data)
        return self.ret_data

    def get_filter_data(self):
        if os.path.exists(f"{DATA_FOLDER}/filters.csv"):
            if datetime.today() - datetime.fromtimestamp(
                os.path.getmtime(f"{DATA_FOLDER}/filters.csv")
            ) < timedelta(days=1):
                return pandas.read_csv(f"{DATA_FOLDER}/filters.csv")

        with open(FORMATS_FILE, "r") as f:
            data = json.load(f)
        symbols = data["symbols"]
        categories = data["categories"]
        df = []
        for symbol in symbols:
            m = yf.Ticker(symbol).info
            d = {"symbol": symbol}
            for category in categories:
                d[category] = m[category]
            df.append(d)
        df = pandas.DataFrame(df)
        df.to_csv(f"{DATA_FOLDER}/filters.csv", index=False)
        return df


class Filter:
    def __init__(self):
        self.saver = Saver()
        self.data = self.saver.get_filter_data()

    def range_filter_bookValue(self, min, max, data=None):
        return self.__range_filter("bookValue", data, min, max)

    def range_filter_averageVolume(self, min, max, data=None):
        return self.__range_filter("averageVolume", data, min, max)

    def range_filter_twoHundredDayAverage(self, min, max, data=None):
        return self.__range_filter("twoHundredDayAverage", data, min, max)

    def __range_filter(self, category, data, min, max):
        if min > max:
            raise ValueError("min should be less than max")
        if type(data) == type(None):
            data = self.data
        return data[(data[category] >= min) & (data[category] <= max)]


def main():
    """
    Arguments should be given as format option **symbols
    """
    s = Saver()
    match sys.argv[1]:
        case "graph":
            Format = formats[sys.argv[2]]
            symbols = sys.argv[3:]
            data = s.save_graph_data(symbols, Format)
            s.save_graph(symbols, data)
        case "filters":
            s.save_filter_data()
        case "download":
            if os.path.exists(DATA_FOLDER):
                for filename in os.listdir(DATA_FOLDER):
                    file_path = os.path.join(DATA_FOLDER, filename)
                    try:
                        if os.path.isfile(file_path) or os.path.islink(file_path):
                            os.unlink(file_path)
                        elif os.path.isdir(file_path):
                            shutil.rmtree(file_path)
                    except Exception as e:
                        print("Failed to delete %s. Reason: %s" % (file_path, e))
                os.rmdir(DATA_FOLDER)
            s = Saver()
        case _:
            print("Wrong option", sys.argv[1])


def test_stock_data_formats_json():
    import json

    with open(FORMATS_FILE, "r") as f:
        data = json.load(f)

    for stock in data["symbols"]:
        m = yf.Ticker(stock).info
        for category in data["categories"]:
            try:
                m[category]
            except Exception as e:
                print(stock, category, e)
                continue

    print("All stocks have all categories!")


def test():
    f = Filter()

    m = f.range_filter_bookValue(300, 600)
    print(m)
    m = f.range_filter_twoHundredDayAverage(400, 600, m)
    print(m)
    m = f.range_filter_averageVolume(16040000, 16041500, m)
    print(m)


if __name__ == "__main__":
    test_stock_data_formats_json()
    test()
