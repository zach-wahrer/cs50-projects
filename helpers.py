import os
import requests
import urllib.parse

from cs50 import SQL
from flask import redirect, render_template, request, session
from functools import wraps

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/1.0/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        response = requests.get(f"https://cloud-sse.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}")
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None

def transactions(t_type):
    """ Get all the transactions of a given type (B or S) """

    final = list()

    # Loop to add up all transactions per distinct stock id for current type
    for row in db.execute("SELECT DISTINCT symbol_id FROM transactions WHERE type = :t_type and user = :user_id", \
                          t_type=t_type, user_id=session["user_id"]):

        # Number of shares bought per id
        q = db.execute("SELECT SUM(shares) FROM transactions WHERE user = :userid AND symbol_id = :symbol_id AND type = :t_type", \
                       userid=session["user_id"], symbol_id=row["symbol_id"], t_type=t_type)
        tmp = (row["symbol_id"], q[0]["SUM(shares)"])
        final.append(tmp)

    return final


def portfolio():
    """ Get user's current portfolio, in a list of ids, shares, price, value, and symbol """

    final = list()
    total_value = 0

    # Iterate through owned stocks / [0] = stock ID, [1] = shares
    for row in stock_owned():

        # Look up stock symbol for given id
        symbol = db.execute("SELECT symbol FROM symbols WHERE id = :symbol_id", symbol_id=row[0])
        # Look up price per share
        quote = lookup(symbol[0]["symbol"])
        # Calculate current value
        value = row[1] * quote["price"]
        # Add it to the tally
        total_value += value

        # Put the values in a final list
        name_symbol = f"{quote['name']} ({symbol[0]['symbol']})"
        tmp = (name_symbol, row[1], usd(quote["price"]), usd(value), symbol[0]["symbol"])
        final.append(tmp)

    final.append(total_value)
    return final

def stock_owned():
    """ Returns a list of owned stocks (stock ID, shares) """
    final = list()

    for bought in transactions("B"):

        shares = bought[1]

        # Iterate through sell transactions to check for sales of bought stocks
        for sold in transactions("S"):
            if bought[0] == sold[0]:
                # Subtract sold shares
                shares -= sold[1]

        if shares >= 1:
            tmp = (bought[0], shares)
            final.append(tmp)

    return final

def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"
