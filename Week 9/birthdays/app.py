import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        person = request.form.get("person")
        if not person:
            return render_template("failure.html", message="missing name")
 
        birthday = request.form.get("birthday")
        if not birthday:
            return render_template("failure.html", message="Missing birthday")
        year, month, day  = [int(b) for b in birthday.split("-")]
        
        db.execute("INSERT INTO")
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        return render_template("index.html")

        # return render_template("index.html", name=person, bday=birthday)
