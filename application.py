import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request, Markup

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():

    # Define variables to import
    innames = ["name", "email", "skill1", "skill2", "skill3", "oddsevens", "birthday", "comments"]

    # Inport the vars into a dict
    invars = dict()
    for i in innames:
        invars[i] = request.form.get(i)

    # Create errors message if required info not present
    if not invars["name"] or not invars["email"]:
        error = "You must enter "
        if not invars["name"] and not invars["email"]:
            error += "name and email address."
        elif not invars["name"]:
            error += "a name."
        elif not invars["email"]:
            error += "an email address."
        error += " Please go back and try again."
        return render_template("error.html", message=error)

    # Open the CSV and write the info to it
    with open('survey.csv', 'a') as file:
        writer = csv.DictWriter(file, fieldnames=innames)
        writer.writerow(invars)

    # Show the user the sheet
    return redirect("/sheet")


@app.route("/sheet", methods=["GET"])
def get_sheet():

    out = list()
#    out = Markup()
    # Open the CSV file
    with open('survey.csv', "r") as file:
        reader = csv.reader(file)

        # Read the file into a list
        for row in reader:
            out.append(row)

    return render_template("sheet.html", data=out)