#cs50 2020 lecture 9 Flask

from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
def index():
    # return "Hello, World!"
    return render_template("index.html")