from logging import StringTemplateStyle
from os import read
from re import escape
from flask import Flask, request
import json

app = Flask(__name__)

@app.route("/<filename>", methods=['GET'])
def subscription(filename):
    if request.method == "GET":
        name = f"{escape(filename)}".replace("\\", "")
        with open(name) as file:
            data = json.loads(file.read())
            return data
    else:
        return "<p>test</p>"