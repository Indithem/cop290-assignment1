import pandas
import datetime
from datetime import date
from dateutil.relativedelta import relativedelta
from jugaad_data.nse import stock_df, NSELive
import sys
from enum import Enum
import requests
import timeit
import os, shutil
import matplotlib.pyplot as plt
import numpy as np
import yaml
import bson

DATA_FOLDER = "stock_data"


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
    REQUIRED_COLUMNS = [
        "DATE",
        "OPEN",
        "CLOSE",
        "HIGH",
        "LOW",
        "LTP",
        "VOLUME",
        "VALUE",
        "NO OF TRADES",
    ]

    def __init__(self, SYMBOL: str) -> None:
        self.SYMBOL = SYMBOL
        self.download_data()

    def download_data(self) -> pandas.DataFrame:
        try:
            df: pandas.DataFrame = stock_df(
                symbol=self.SYMBOL,
                from_date=date.today() - relativedelta(years=10),
                to_date=date.today(),
            )
            df = df[self.REQUIRED_COLUMNS]
            self.data: pandas.DataFrame = df
            return df
        except Exception as e:
            self.data = pandas.DataFrame()
            print(f"Error during downloading {self.SYMBOL} data\n{e}")

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
                    f"stock_data/{self.SYMBOL}{test_name}.{fileformat}"
                ).st_size
                benchmark_results.append((f"{fileformat}{test_name}", time, size))
        return benchmark_results

    def write_to_csv(self) -> None:
        self.data.to_csv(f"stock_data/{self.SYMBOL}.csv", index=False)

    # def write_to_txt_asStr(self) -> None:
    #     with open(f"stock_data/{self.SYMBOL}_asStr.txt", "w") as f:
    #         f.write(self.data.to_string(index=False))

    def write_to_txt(self) -> None:
        self.data.to_csv(f"stock_data/{self.SYMBOL}.txt", index=False, sep="\t")

    # def write_to_json_records(self) -> None:
    #     self.data.to_json(f"stock_data/{self.SYMBOL}_records.json", orient="records")

    # def write_to_json_columns(self) -> None:
    #     self.data.to_json(f"stock_data/{self.SYMBOL}_columns.json", orient="columns")

    # def write_to_json_split(self) -> None:
    #     self.data.to_json(f"stock_data/{self.SYMBOL}_split.json", orient="split")

    # def write_to_json_index(self) -> None:
    #     self.data.to_json(f"stock_data/{self.SYMBOL}_index.json", orient="index")

    def write_to_json(self) -> None:
        # json_split is found to be the best
        self.data.to_json(f"stock_data/{self.SYMBOL}.json", orient="values")

    # def write_to_json_table(self) -> None:
    #     self.data.to_json(f"stock_data/{self.SYMBOL}_table.json", orient="table")

    def write_to_xlsx(self) -> None:
        self.data.to_excel(f"stock_data/{self.SYMBOL}.xlsx", index=False)

    def write_to_html(self) -> None:
        self.data.to_html(f"stock_data/{self.SYMBOL}.html", index=False)

    def write_to_tex(self) -> None:
        self.data.to_latex(f"stock_data/{self.SYMBOL}.tex", index=False)

    def write_to_xml(self) -> None:
        self.data.rename(lambda x: x.replace(" ", "_"), axis=1).to_xml(
            f"stock_data/{self.SYMBOL}.xml", index=False
        )

    def write_to_feather(self) -> None:
        self.data.to_feather(f"stock_data/{self.SYMBOL}.feather")

    def write_to_parquet(self) -> None:
        self.data.to_parquet(f"stock_data/{self.SYMBOL}.parquet")

    def write_to_orc(self) -> None:
        self.data.to_orc(f"stock_data/{self.SYMBOL}.orc")

    def write_to_dta(self) -> None:
        self.data.rename(lambda x: x.replace(" ", "_"), axis=1).to_stata(
            f"stock_data/{self.SYMBOL}.dta", write_index=False
        )

    def write_to_hdf(self) -> None:
        self.data.to_hdf(f"stock_data/{self.SYMBOL}.hdf", key="data", mode="w")

    def write_to_pkl(self) -> None:
        self.data.to_pickle(f"stock_data/{self.SYMBOL}.pkl")

    def write_to_yaml(self) -> None:
        yaml.dump(self.data.to_dict(), open(f"stock_data/{self.SYMBOL}.yaml", "w"))

    def write_to_bson(self) -> None:
        with open(f"stock_data/{self.SYMBOL}.bson", "wb") as f:
            f.write(bson.dumps(self.data.to_dict()))


class formats(Enum):
    daily = 1
    weekly = 2
    monthly = 3
    quarterly = 4
    yearly = 5


class Saver:
    def __init__(self):
        if not os.path.exists("stock_data"):
            os.makedirs("stock_data")
            stocks = self.get_csv_data()
            for stock in stocks:
                if not os.path.exists(f"stock_data/{stock}.orc"):
                    p = Downloader(stock)
                    p.write_to_orc()

    def save_graph(self, data: list[pandas.DataFrame], symbols):
        fig, ax = plt.subplots(figsize=(10, 5))
        for i, d in enumerate(data):
            ax.plot(d["DATE"], d["LTP"], label=symbols[i])
        ax.set_xlabel("DATE")
        ax.set_ylabel("LTP")
        ax.legend()
        fig.tight_layout()
        fig.savefig(f"stock_data/out.png", dpi=400)

    def save_graph_data(self, symbols, format):
        ret_data = []
        for symbol in symbols:
            if not os.path.exists(f"stock_data/{symbol}.orc"):
                p = Downloader(symbol)
                data = p.data
                p.write_to_orc()
            else:
                data = pandas.read_orc(f"stock_data/{symbol}.orc")
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
            ret_data.append(data)
        return ret_data

    def get_csv_data(self):
        if not os.path.exists(f"stock_data/nifty_list.csv"):
            r = requests.get(
                "https://drive.google.com/uc?export=download&id=15ZAkp5fo7bd7VniWNZQRIhU6LhPLR2kH",
                allow_redirects=True,
            )
            open(f"stock_data/nifty_list.csv", "wb").write(r.content)
        data = pandas.read_csv(f"stock_data/nifty_list.csv")
        return data["Symbol"]

    def save_filter_data(self):
        syms = self.get_csv_data()
        data = dict()
        nse_live = NSELive()
        for stock_sym in syms:
            data[stock_sym] = nse_live.stock_quote(stock_sym)["priceInfo"]["close"]
        data = pandas.DataFrame(data.items(), columns=["Symbol", "Price"])
        data.to_csv(f"stock_data/stock_prices.csv", index=False)


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
            s.save_graph(data, symbols)
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


if __name__ == "__main__":
    main()
