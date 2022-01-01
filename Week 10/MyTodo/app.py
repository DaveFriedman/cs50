import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

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

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///todo.db")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():

    if request.method == "POST":
        if not request.form.get("isdone"):
          return apology("error", 404)
        
        else:
            title = request.form.get("isdone")
            db.execute(
                "UPDATE tasks SET isdone = 1 WHERE title = ?",
                title)

            tasks = db.execute(
                "SELECT title, desc, created, due \
                FROM tasks \
                WHERE isdone = 0 AND isdeleted = 0 AND userid = ?",
                session["user_id"])

            user = db.execute(
                "SELECT username FROM users WHERE id = ?", session["user_id"])
            name = user[0]["username"]

            return render_template("index.html", user=name, tasks=tasks)

    else:
        if "user_id" not in session:
            session["user_id"] = []

        user = db.execute(
            "SELECT username FROM users WHERE id = ?", session["user_id"])

        name = user[0]["username"]

        tasks = db.execute(
            "SELECT title, desc, created, due \
            FROM tasks \
            WHERE isdone = FALSE AND isdeleted = FALSE AND userid = ?",
            session["user_id"])

        return render_template("index.html", user=name, tasks=tasks)


@app.route("/account", methods=["GET", "POST"])
@login_required
def account():

    if request.method == "POST":
        if request.form.get("username"):
            db.execute(
                "UPDATE users SET username = ? WHERE id = ?", 
                request.form.get("username"), session["user_id"])
        
        if request.form.get("email"):
            db.execute(
                "UPDATE users SET email = ? WHERE id = ?", 
                request.form.get("email"), session["user_id"])

        if request.form.get("password"):
            db.execte(
                "UPDATE users SET hash = ? WHERE id =?",
                generate_password_hash(request.form.get("password")), 
                session["user_id"])

        flash("You have successfully updated your account")
        return redirect("/")

    else:
        row = db.execute(
            "SELECT username, email FROM users WHERE id =?", session["user_id"])
        username, email = row[0]["username"], row[0]["email"]

        return render_template("account.html", username=username, email=email)


@app.route("/newtask", methods=["GET", "POST"])
@login_required
def newtask():

    if request.method == "POST":

        if not request.form.get("task"):
            return apology("no task", 400)
            
        db.execute(
            "INSERT INTO tasks \
            (title, desc, created, due, userid) \
            VALUES(?, ?, ?, ?, ?)",
            request.form.get("task"),
            request.form.get("desc"),
            datetime.now(),
            request.form.get("due"),
            session["user_id"])

        flash("You have added a new task")
        return redirect("/")

    else:

        return render_template("newtask.html")


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
        username = request.form.get("username")
        hash = generate_password_hash(request.form.get("password"))
        email = request.form.get("email")
        usercheck = db.execute(
            "SELECT * FROM users WHERE username = ?", username)

        # Ensure username isn't already taken
        if usercheck:
            if username in usercheck[0]["username"]:
                flash("username unavailable, please choose another")
                return redirect("/register")
        else:
            db.execute(
                "INSERT INTO users (username, hash, email, created) \
                VALUES(?, ?, ?, ?)",
                username, hash, email, datetime.now())

            # Remember which user has logged in
            newuser = db.execute(
                "SELECT * FROM users WHERE username = ?", username)

            session["user_id"] = newuser[0]["id"]

            # Redirect user to home page
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
