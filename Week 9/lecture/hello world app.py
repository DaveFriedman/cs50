#cs50 2020 lecture 9 Flask

from flask import Flask, render_template, request

app = Flask(__name__)

@app.route("/")
def index():
    # return "Hello, World!"
    # return render_template("index.html")
    return render_template("index.html", name=request.args.get("name", "world"))