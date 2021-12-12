#cs50 2020 lecture 9 Flask

from flask import Flask, render_template, request

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/greet", methods=["GET", "POST"])
def greet():
    # return render_template("greet.html", name=request.args.get("name", "World"))

    # the form in index.html will always return something, even just an empty  
    # string, so the default param "World" in request.form.get() is never used.
    # This is solved by using OR, which sees an empty string as False 
    return render_template("greet.html", name=request.form.get("name") or "World")
