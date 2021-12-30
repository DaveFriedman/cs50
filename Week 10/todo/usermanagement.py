from flask import redirect, session
from functools import wraps

def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def isvalidusername(name):
    """ Usernames must be:
        Between 2 and 20 chars"""
    length = len(name)
    if 2 <= len(name) <= 20:
        return True
    return False


def isvalidpassword(pw):
    """ Passwords must be:
        Between 5 and 50 chars"""
    if 5 <= len(pw) <= 50:
        return True
    return False


def isvalidemail(email):
    if "@" in email:
        return True
    return False