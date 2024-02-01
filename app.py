from flask import Flask, render_template, request, redirect, url_for, flash, session
from flask_sqlalchemy import SQLAlchemy
from werkzeug.security import generate_password_hash, check_password_hash

from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField,validators
from wtforms.validators import Length

import getter

app = Flask(__name__)
app.secret_key = "your_secret_key"  # Replace with your actual secret key
app.config["WTF_CSRF_ENABLED"] = False

# Database Configuration
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///users.db"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
db = SQLAlchemy(app)


# User Model
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(100), unique=True, nullable=False)
    password_hash = db.Column(db.String(200), nullable=False)


# Password Validator
class PasswordValidator(FlaskForm):
    password = StringField("password", validators=[
        validators.Length(min=8, message="Password must be at least 8 characters long."),
        validators.Regexp(
            regex=r'^(?=.*[a-z])',
            message="Password must contain at least one lowercase letter."
        ),
        validators.Regexp(
            regex=r'^(?=.*[A-Z])',
            message="Password must contain at least one uppercase letter."
        ),
        validators.Regexp(
            regex=r'^(?=.*\d)',
            message="Password must contain at least one number."
        ),
        validators.Regexp(
            regex=r'^(?=.*[@$!%*?&])',
            message="Password must contain at least one special character."
        )
    ])

# Initialize Database within Application Context
with app.app_context():
    db.create_all()


@app.route("/")
def index():
    return render_template("home.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form["username"]
        password_validator = PasswordValidator(request.form)
        if not password_validator.validate():
            for error in password_validator.errors.values():
                flash(error[0])
            return redirect(url_for("register"))
      
        password = request.form["password"]
        hashed_password = generate_password_hash(password, method="pbkdf2:sha256")
        user = User.query.filter_by(username=username).first()
        if user :
            flash("Username already exists") 
            return redirect(url_for("register"))
        new_user = User(username=username, password_hash=hashed_password)
        db.session.add(new_user)
        db.session.commit()

        flash("Registration successful! Please login.")
        return redirect(url_for("login"))

    return render_template("register.html")


@app.route("/login", methods=["POST","GET"])
def login():
    if "user_id" in session:
        return redirect(url_for("dashboard"))
    if request.method=="GET":
        return render_template("login.html")
    username = request.form["username"]
    password = request.form["password"]
    user = User.query.filter_by(username=username).first()

    if user and check_password_hash(user.password_hash, password):
        session["user_id"] = user.id
        session["username"] = user.username
        return redirect(url_for("dashboard"))
    else:
        flash("Invalid username or password")
        return redirect(url_for("login"))


@app.route("/dashboard")
def dashboard():
    if "user_id" in session:
        return render_template("welcome.html", username=session["username"])
    else:
        return redirect(url_for("index"))


@app.route("/logout")
def logout():
    session.pop("user_id", None)
    session.pop("username", None)
    return redirect(url_for("index"))

@app.route("/stocks", methods=["GET"])
def stocks():
    """
    Renderes the page for stocks.


    Gets a HTTP POST request from the form in the stocks page.
    The first time request of all the filters is taken.
    The second time request is taken for the stock symbols.

    `stocks` list consists of the all the filtered stock symbols, starting from index 1.
    (0th index is ignored to flag that filters request is received but none match the filters)
    """
    if "user_id" not in session:
        flash("Please login first!")
        return render_template("login.html")
    f = getter.Filter()
    ranges = dict()
    ranges['avg'] = f.get_range_twoHundredDayAverage()
    ranges['vol'] = f.get_range_averageVolume()
    return render_template("stocks.html",plot_data='None', stocks=[], ranges=ranges)

           
@app.route("/stocks_get_graph", methods=["POST"])
def get_stocks_graph():
    print(request.form)
    symbols = [stock for stock in request.form if request.form[stock] == "on"]

    saver = getter.Saver()
    saver.get_graph_data(symbols, getter.formats[request.form['frequency']])

    print("Done making graph!")
    return saver.get_graph()


@app.route("/stocks_get_list", methods=["POST"])
def get_stocks_list():
    print(request.form)
    f = getter.Filter()
    sort_descending = 'sortOption' in request.form and request.form['sortOption'] == 'highToLow'
    f.range_filter_twoHundredDayAverage(
        request.form["averagePriceMin"],
        request.form["averagePriceMax"]
    )
    f.range_filter_averageVolume(
        request.form["Vol_min"],
        request.form["Vol_max"]
    )
    match request.form["sort_by"]:
        case "Average Price":
            f.sort_filter_twoHundredDayAverage(not sort_descending)
        case "Volume":
            f.sort_filter_averageVolume(not sort_descending)
        case "Book value":
            f.sort_filter_bookValue(not sort_descending)
    stocks = [None] + f.data.to_dict(orient="records")
    return render_template("stocks_layout.html", stocks=stocks)
        


if __name__ == "__main__":
    app.run(debug=True)
