import sqlite3

from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from markupsafe import escape
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash, safe_str_cmp

from usermanagement import login_required, isvalidusername, isvalidpassword, isvalidemail

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

db = sqlite3.connect("todo.db")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():

    if request.method == "POST":
        return render_template("login.html")

    else:
        if "user_id" not in session:
            session["user_id"] = []
        
        user = db.execute(
            "SELECT username FROM users WHERE id = ?", session["user_id"])

        return render_template("index.html", user=user)

@app.route("/login", methods=["GET", "POST"])
def login():

    session.clear()

    if request.method == "POST":

        # Input: username, password
        # Ensure all inputs exist and are valid
        # If username:password pair exists, send that user to index

        if request.form.get("username"):
            username = escape(request.form.get("username"))
            if not isvalidusername(username):
                flash("invalid username", "error")
        else:
            flash("no username", "error")

        if request.form.get("password"):
            password = request.form.get("password")
            if not isvalidpassword(password):
                flash("invalid password", "error")
        else:
            flash("no password", "error")

        auth = db.execute(
            "SELECT * FROM users WHERE username = ?", username)
        
        if len(auth) != 1 or not check_password_hash(auth[0]["hash"], password):
            flash("invalid username and/or password", "error")

        session["user_id"] = auth[0]["id"]

        return redirect("/")

    else:
        return render_template("login.html")


@app.route("/logout", methods=["GET", "POST"])
def logout():
    
    session.clear()

    return redirect("/")

@app.route("/register", methods=["GET", "POST"])
def register():
    session.clear()

    if request.method == "POST":

        # Input: username, password, confirmation, email address
        # Ensure all inputs exist and are valid
        # Ensure password matches confirmation
        # Ensure username isn't taken
        # If all is good: add user to db, create session, send user to index

        if request.form.get("username"):
            username = escape(request.form.get("username"))
            if not isvalidusername(username):
                flash("invalid username", "error")
        else:
            flash("no username", "error")

        if request.form.get("password"):
            password = request.form.get("password")
            if not isvalidpassword(password):
                flash("invalid password", "error")
        else:
            flash("no password", "error")

        if request.form.get("confirmation"):
            confirmation = request.form.get("confirmation")
        else:
            flash("no confirmation", "error")
        
        if safe_str_cmp(password, confirmation):
            hash = generate_password_hash(password)

        if request.form.get("email"):
            email = escape(request.form.get("email"))
            if not isvalidemail(email):
                flash("invalid email", "error")
        else:
            flash("no email", "error")

        usercheck = db.execute(
            "SELECT * FROM users WHERE username = ?", username)
        
        if usercheck:
            if username in usercheck[0]["username"]:
                flash("username taken", "error")
        else:
            db.executemany(
                "INSERT INTO user (username, hash, email, joined) \
                VALUES(?, ?, ?, ?)",
                username, hash, email, datetime.now())

            thisuser = db.execute(
                "SELECT * FROM users WHERE username = ?", username)
            
            session["user_id"] = thisuser[0]["id"]

            return redirect("/")

    else:
        return render_template("register.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return render_template(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)