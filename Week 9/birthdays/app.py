import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, \
                  render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        name = request.form.get("theirname")
        if not name:
            return render_template("failure.html", message="missing name")
 
        birthday = request.form.get("birthday")
        if not birthday:
            return render_template("failure.html", message="Missing birthday")

        year, month, day  = [int(b) for b in birthday.split("-")]
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", \
                                            name, month, day)

        friends = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", friends=friends)

    else:
        friends = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", friends=friends)
