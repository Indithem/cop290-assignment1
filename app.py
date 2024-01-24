from flask import Flask, render_template, request, redirect, url_for, flash, session
from flask_sqlalchemy import SQLAlchemy
from werkzeug.security import generate_password_hash, check_password_hash

from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField
from wtforms.validators import Length

app = Flask(__name__)
app.secret_key = "your_secret_key"  # Replace with your actual secret key

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
class Password_validator(FlaskForm):
    password = PasswordField("password", validators=[
        Length(min=8, message="Password must be atleast 8 characters long.")
    ])


# Initialize Database within Application Context
with app.app_context():
    db.create_all()


@app.route("/")
def index():
    return render_template("login.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        password_validator = Password_validator()
        username = request.form["username"]
        password = request.form["password"]
        password_validator.password.data = password
        if not password_validator.validate():
            flash("Password must be atleast 8 characters lobjnkmng.")
            return redirect(url_for("register"))

        hashed_password = generate_password_hash(password, method="pbkdf2:sha256")

        new_user = User(username=username, password_hash=hashed_password)
        db.session.add(new_user)
        db.session.commit()

        flash("Registration successful! Please login.")
        return redirect(url_for("index"))

    return render_template("register.html")


@app.route("/login", methods=["POST"])
def login():
    username = request.form["username"]
    password = request.form["password"]
    user = User.query.filter_by(username=username).first()

    if user and check_password_hash(user.password_hash, password):
        session["user_id"] = user.id
        session["username"] = user.username
        return redirect(url_for("dashboard"))
    else:
        flash("Invalid username or password")
        return redirect(url_for("index"))


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


if __name__ == "__main__":
    app.run(debug=True)
