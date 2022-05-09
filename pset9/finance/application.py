import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

    # Get user's stocks
    stocks = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = ? GROUP BY symbol", session["user_id"])

    total_money = 0

    # Add the name, price and total with API to every stock and increment total money
    for stock in stocks:
        api = lookup(stock["symbol"])
        stock["name"] = api["name"]
        stock["price"] = api["price"]
        stock["total"] = stock["price"] * stock["shares"]
        total_money += stock["total"]

    # Get user's cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
    total_money += cash

    return render_template("index.html", stocks=stocks, cash=cash, total=total_money)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)

        # Get stock information with API
        stock = lookup(request.form.get("symbol"))

        # Check if symbol exist
        if stock == None:
            return apology("Symbol don't exist", 400)

        # Ensure shares is a valid number
        if not request.form.get("shares").isdigit():
            return apology("invalid number", 400)

        # Total money that will be spent
        shares = int(request.form.get("shares"))
        total = stock["price"] * shares

        # Cash avaible for the user
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']

        # Check if user have enought money
        if total > cash:
            return apology("You don't have enought money")

        # If user already have that stock
        row = db.execute("SELECT shares FROM stocks WHERE user_id = ? AND symbol = ?", session["user_id"], stock["symbol"])
        if len(row) > 0:
            total_shares = row[0]["shares"]
            total_shares += shares

            # Update the number of shares by adding to the stored number of shares
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?",
                       int(total_shares), session["user_id"], stock["symbol"])
        else:
            # Save user's stocks
            db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?)", session["user_id"], stock["symbol"], shares)

        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - total, session["user_id"])

        # Update transaction history
        db.execute("INSERT INTO history (user_id, symbol, shares, price, action, timestamp) VALUES (?, ?, ?, ?, 'buy', datetime('now'))",
                   session["user_id"], stock["symbol"], shares, stock["price"])

        flash('Bought!')

        return redirect("/")

    else:
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get stocks from history table
    stocks = db.execute("SELECT * FROM history WHERE user_id = ?", session["user_id"])

    # Add total of cash, and symbol "+" or "-"
    for stock in stocks:
        if stock["action"] == "buy":
            stock["total"] = "-" + usd(stock["price"] * stock["shares"])
        else:
            stock["total"] = "+" + usd(stock["price"] * stock["shares"])

    return render_template("/history.html", stocks=stocks)


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

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        flash('Logged in!')

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

    flash('Logged out!')

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("Must provide symbol", 400)

        # Get stock information with API
        stock = lookup(request.form.get("symbol"))

        # Check if symbol exist
        if stock == None:
            return apology("Symbol don't exist")

        return render_template("/quoted.html", stock=stock)

    # User reached route via GET
    else:
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        if not request.form.get("confirmation"):
            return apology("must provide password", 400)

        # Ensure passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Ensure username don't already exist
        if db.execute("SELECT username FROM users WHERE username = ?", request.form.get("username")):
            return apology("username already exist", 400)

        # Create new user in table
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        # Query database for user's id
        row = db.execute("SELECT id FROM users WHERE username = ?", request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = row[0]["id"]

        flash('Registered!')

        # Redirect user to login page
        return render_template("/greed.html", username=request.form.get("username"))

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Get user's stocks information
    stocks = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = ?", session["user_id"])

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Store user's stocks symbols
        symbols = []
        for stock in stocks:
            symbols.append(stock["symbol"])

        symbol_sell = request.form.get("symbol")
        # Ensure is a valid symbol
        if symbol_sell not in symbols:
            return apology("symbol not owned", 400)

        # Store total number of shares of requested symbol
        total_shares = 0
        for stock in stocks:
            if stock["symbol"] == symbol_sell:
                total_shares = stock["shares"]
                break
        # Ensure user have enough shares
        shares_sell = int(request.form.get("shares"))
        if shares_sell > total_shares:
            return apology("too many shares")

        if shares_sell == total_shares:

            # If user will sell all of stocks, delete from table
            db.execute("DELETE FROM stocks WHERE symbol = ? AND user_id = ?", symbol_sell, session["user_id"])

        else:
            # Update number of shares of sold stock
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?",
                       total_shares - shares_sell, session["user_id"], symbol_sell)

        # Cash avaible for the user
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']

        # Cash from the sold stocks
        cash_sell = lookup(symbol_sell)["price"] * shares_sell

        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash + cash_sell, session["user_id"])

        # Update transaction history
        db.execute("INSERT INTO history (user_id, symbol, shares, price, action, timestamp) VALUES (?, ?, ?, ?, 'sell', datetime('now'))",
                   session["user_id"], symbol_sell, shares_sell, cash_sell)

        flash('Sold!')

        return redirect("/")

    # User reached route via GET
    else:
        return render_template("/sell.html", stocks=stocks)


# Add or remove cash from user
@app.route("/changecash", methods=["POST"])
@login_required
def change_cash():

    amount = float(request.form.get("amount"))

    # Get user's cash
    cash = float(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash'])

    # Update user's cash
    db.execute("UPDATE users SET cash =  ? WHERE id = ?", cash + amount, session["user_id"])

    # Provide feedback
    if amount > 0:
        flash('Cash added!')
    else:
        flash('Cash removed!')

    return redirect("/")


# Greet the user and get initial cash
@app.route("/greet", methods=["POST"])
@login_required
def greet():

    # Get amount of money from the user
    cash = float(request.form.get("cash"))

    # Update user's cash
    db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])

    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
