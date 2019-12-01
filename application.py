import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, transactions, portfolio, stock_owned

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
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Look up username and cash for the webpage
    userdata = db.execute("SELECT username, cash FROM users WHERE id = :userid", userid=session["user_id"])

    # Get the current portfolio
    stocks = portfolio()
    # Get the current total value of the stocks
    total_value = stocks.pop()

    return render_template("index.html", username=userdata[0]["username"], cash=usd(userdata[0]["cash"]), \
                           stocks=stocks, total_value=usd(total_value), total=usd(userdata[0]["cash"] + total_value))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Buy if info is submitted
    if request.method == "POST":

        # Check shares input
        if not request.form.get("shares").isdigit() or int(request.form.get("shares")) <= 0:
            return apology("You must enter a valid amount of shares.", 400)

        # Lookup symbol info
        stock = lookup(request.form.get("symbol"))

        # Check if symbol exists
        if stock == None:
            return apology("That symbol does not exist.", 400)

        # Get current cash and set some vars
        db_return = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        value = stock["price"] * int(request.form.get("shares"))

        # Check to make sure user has enough money for transaction
        if db_return[0]["cash"] < value:
            return apology(f"You cannot afford that many shares of {stock['name']}.")

        # Do the transaction if the user's funds are successfully decremented
        if db.execute("UPDATE users SET cash = :remaining WHERE id = :user_id", \
                      remaining='%.2f'%(db_return[0]["cash"] - value), user_id=session["user_id"]) != None:

            # Look up the symbol ID
            symbolid = db.execute("SELECT id FROM symbols WHERE symbol = :symbol", symbol=stock["symbol"])
            # If it doesn't exist, add it
            if not symbolid:
                finalid = db.execute("INSERT INTO symbols (symbol) VALUES (:symbol)", symbol=stock["symbol"])
            else:
                finalid = symbolid[0]["id"]

            # Put the transaction into the database
            if db.execute("INSERT INTO transactions (user, symbol_id, type, shares, price) VALUES (:user, :symbol, 'B', :shares, :price)", \
                          user=session["user_id"], symbol=finalid, shares=int(request.form.get("shares")), price=stock["price"]) <= 0:

                # Restore balance if transaction unsuccessful
                db.execute("UPDATE users SET cash = :original WHERE id = :user_id", original=db_return[0]['cash'], user_id=session["user_id"])
                # Report error
                return apology("Your transaction could not be processed.", 500)



        # Look up username and cash for the webpage
        userdata = db.execute("SELECT username, cash FROM users WHERE id = :userid", userid=session["user_id"])

        # Return success via flash
        flash(f"You bought {usd(value)} worth of {stock['name']} shares. {usd(userdata[0]['cash'])} available.")

        # Get the current portfolio
        stocks = portfolio()
        # Get the current total value of the stocks
        total_value = stocks.pop()

        return render_template("index.html", username=userdata[0]["username"], cash=usd(userdata[0]["cash"]), \
                           stocks=stocks, total_value=usd(total_value), total=usd(userdata[0]["cash"] + total_value))

    # Show the buy form if no info submitted
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""

    # Check for username in database
    if db.execute("SELECT id FROM users WHERE username = :username", username=request.args.get("username")) or not request.args.get("username"):
        return jsonify(False)

    return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Look up username the webpage
    userdata = db.execute("SELECT username FROM users WHERE id = :userid", userid=session["user_id"])

    # Get all transactions
    stocks = db.execute("SELECT symbol, type, shares, price, date FROM transactions JOIN symbols ON symbols.id = transactions.symbol_id WHERE user = :user_id", \
                          user_id=session["user_id"])
    # Exit if user has no transactions
    if not stocks:
        return render_template("history.html", username=userdata[0]["username"])

    return render_template("history.html", username=userdata[0]["username"], stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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

    # Display quote if POSTed data
    if request.method == "POST":

        # Make sure input isn't blank
        if request.form.get("symbol"):
            # Get input
            data = lookup(request.form.get("symbol"))
        else:
            return apology("You have to enter a symbol.", 400)

        # Flash if symbol unknown
        if data == None:
            return apology("Invalid symbol.", 400)

        return render_template("quote.html", data=data)

    # Show input form if GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Check if data was submitted
    if request.method == "POST":

        # Error checking
        if not request.form.get("username"):
            return apology("You must enter a username.")
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("You must enter a password and confirm it.")
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Your passwords didn't match.")

        # Insert user/pass into database
        check = db.execute("INSERT INTO 'users' ('username', 'hash') VALUES (:user, :hashword)", \
                       user=request.form.get("username"), hashword=generate_password_hash(request.form.get("password")))

        # Check for duplicate username
        if not check:
            return apology("That username is taken. Please try another.", 400)
        else:
            flash('Account created successfully. Please log in.')

        # Send them to log in
        return render_template("login.html")

    # Show register form if no data submitted
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Check if data submitted
    if request.method == "POST":

        # Check if stock selected and shares in valid format
        if request.form.get("symbol") == "Company" or not request.form.get("shares").isdigit() or int(request.form.get("shares")) <= 0:
            return apology("You must select a stock and valid quantity to sell.", 403)

        # Check to see if user can actually sell the stocks
        try:
            q = db.execute("SELECT id FROM symbols WHERE symbol = :symbol", symbol=request.form.get("symbol"))
        except:
                return apology("Your transaction couldn't be completed.", 400)

        for row in stock_owned():
            if row[0] == q[0]["id"]:
                # Error if user tries to sell more shares than they have available
                if int(request.form.get("shares")) > row[1]:
                    return apology(f"You do not have that many shares of {request.form.get('symbol')} to sell", 400)
                # Do the transaction
                else:
                    check = lookup(request.form.get("symbol"))
                    insert = db.execute("INSERT INTO transactions (user, symbol_id, type, shares, price) VALUES (:user, :symbol_id, 'S', :shares, :price)", \
                                   user=session["user_id"], symbol_id=row[0], shares=request.form.get("shares"), price=check["price"])
                    # Update available cash
                    if insert > 0:
                        # Get updated cash amount and change database
                        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
                        sale = (int(request.form.get("shares")) * check["price"])
                        new_cash = cash[0]["cash"] + sale
                        update = db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id", new_cash=new_cash, user_id=session["user_id"])

                        flash(f"You successfully sold {usd(sale)} worth of {request.form.get('symbol')}")
                        return redirect("/sell")

                    # Error if cash couldn't be updated and delete the sell
                    else:
                        db.execute("DELETE FROM transactions where id = :trans_id", trans_id=insert)
                        return apology("Your transaction couldn't be completed.", 500)

        # If it makes it to the end without finding a stock to sell
        return apology("Your transaction couldn't be completed.", 400)


    # Make sell form if no data submitted
    else:
        final_list = list()
        # Get currently owned symbols for sell form
        for row in stock_owned():
            q = db.execute("SELECT symbol FROM symbols WHERE id = :stock_id", stock_id=row[0])
            final_list.append(q[0]["symbol"])

        final_list.sort()
        return render_template("sell.html", stocks_owned=final_list)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
