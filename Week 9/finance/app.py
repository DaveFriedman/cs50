import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
# to use iex sandbox: with open('iexsandboxtoken.txt') as f:
with open('iextoken.txt') as f:
    key = f.read()
os.environ["API_KEY"] = key
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():

    if request.method == "POST":
        return render_template("login.html")

    else:
        if "user_id" not in session:
            session["user_id"] = []

        user = db.execute(
            "SELECT username, cash FROM users WHERE id = ?", session["user_id"])

        name, cash = user[0]["username"], user[0]["cash"]

        portfolio = db.execute(
            "SELECT symbol, companyName, sum(shares) AS totalshares \
            FROM transactions \
            WHERE userid = ? \
            GROUP BY symbol \
            HAVING shares > 0",
            session["user_id"])

        balance = cash
        for stock in portfolio:
            symbol = stock["symbol"]
            current = lookup(symbol)
            stock["value"] = int(current["price"]) * stock["totalshares"]
            balance += stock["value"]
            stock["value"] = usd(stock["value"])
            stock["price"] = usd(current["price"])

        cash, balance = usd(cash), usd(balance)

        return render_template("index.html",
            user=name, cash=cash, balance=balance, portfolio=portfolio)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("no symbol", 400)
        if not request.form.get("shares"):
            return apology("no shares", 400)

        shares = request.form.get("shares")
        numbers = '0123456789'
        if set(shares).difference(numbers):
            return apology("shares must be integers", 400)

        shares = int(shares)
        if shares < 1:
            return apology("can't buy fewer than 1 share", 400)

        symbol = request.form.get("symbol")
        try:
            quote = lookup(symbol)
            companyName, symbol = quote["name"], quote["symbol"]
            price = quote["price"]
        except:
            return apology("symbol not found", 400)

        user = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"])

        cash = user[0]["cash"]
        cost = shares * price

        if cost > cash:
            return apology("Insufficient funds", 400)
        else:
            cash = round(cash-cost, 2)
            db.execute(
                "UPDATE users SET cash = ? where id = ?",
                cash, session["user_id"])
            db.execute(
                "INSERT INTO transactions \
                (userid, symbol, companyName, price, shares, cost, timestamp) \
                VALUES(?, ?, ?, ?, ?, ?, ?)",
                session["user_id"],
                symbol,
                companyName,
                price,
                shares,
                cost,
                datetime.now())
            return redirect("/")

    else:
        user = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"])

        return render_template("buy.html", cash=usd(user[0]["cash"]))


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    trades = db.execute(
        "SELECT symbol, companyName, price, shares, cost, timestamp \
        FROM transactions \
        WHERE userid = ?",
        session["user_id"])

    for trade in trades:
        trade["price"] = usd(trade["price"])
        trade["cost"] = usd(trade["cost"])

    return render_template("history.html", trades=trades)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        else:
            username = request.form.get("username")

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)
        else:
            password = request.form.get("password")

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", username)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("no symbol", 400)

        symbol = request.form.get("symbol")
        try:
            quote = lookup(symbol)
            name, symbol = quote["name"], quote["symbol"]
            price = usd(quote["price"])
        except:
            return apology("symbol not found", 400)

        if quote:
            return render_template(
                "quoteshow.html", name=name, symbol=symbol, price=price)
        else:
            return render_template("quoteshow.html")

    else:
        return render_template("quoteget.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Query database for username
        username = request.form.get("username").strip(";")
        hash = generate_password_hash(request.form.get("password"))
        usercheck = db.execute(
            "SELECT * FROM users WHERE username = ?", username)

        # Ensure username isn't already taken
        if usercheck:
            if username in usercheck[0]["username"]:
                return apology("username taken", 400)
        else:
            db.execute(
                "INSERT INTO users (username, hash) VALUES(?, ?)",
                username, hash)

            # Remember which user has logged in
            newuser = db.execute(
                "SELECT * FROM users WHERE username = ?", username)

            session["user_id"] = newuser[0]["id"]

            # Redirect user to home page
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))

        portfolio = db.execute(
            "SELECT symbol, sum(shares) \
            FROM transactions \
            WHERE userid = ? AND symbol = ?",
            session["user_id"], symbol)

        if symbol != portfolio[0]["symbol"]:
            return apology(f"{symbol} not found")

        owned = portfolio[0]["sum(shares)"]

        if shares > owned:
            return apology(f"You have {owned} shares")

        quote = lookup(symbol)

        if not quote:
            return apology("No such stock symbol", 400)
        else:
            companyName, symbol = quote["name"], quote["symbol"]
            price = quote["price"]

            user = db.execute(
                "SELECT cash FROM users WHERE id = ?", session["user_id"])

            cash = user[0]["cash"]
            revenue = round(shares * price, 2)
            cash += revenue

            db.execute(
                "UPDATE users SET cash = ? where id = ?",
                cash, session["user_id"])

            db.execute(
                "INSERT INTO transactions \
                (userid, symbol, companyName, price, shares, cost, timestamp) \
                VALUES(?, ?, ?, ?, ?, ?, ?)",
                session["user_id"],
                symbol,
                companyName,
                price,
                -shares,
                -revenue,
                datetime.now())

            return redirect("/")

    else:
        portfolio = db.execute(
            "SELECT symbol FROM transactions WHERE userid = ? GROUP BY symbol",
            session["user_id"])

        return render_template("sell.html", portfolio=portfolio)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
