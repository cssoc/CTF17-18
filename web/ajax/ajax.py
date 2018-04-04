from flask import Flask, render_template, request, jsonify
import sqlite3

app = Flask(__name__)
con = sqlite3.connect('/db/db.sqlite')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/get')
def get():
    ID = request.args.get('id', type=int)
    c = con.cursor()
    c.execute('SELECT * FROM messages WHERE rowid=?', (ID,))
    res = c.fetchall()
    c.close()
    return jsonify(res[0])

@app.route('/api/check')
def check():
    recp = request.args.get('recp', type=str)
    c = con.cursor()
    c.execute('SELECT rowid FROM messages WHERE recp=? ORDER BY rowid ASC', (recp,))
    res = []
    for x in  c.fetchall():
        res.append(x[0])
    c.close()
    return jsonify(res)

@app.route('/api/send')
def send():
    recp = request.args.get('recp', type=str)
    sndr = request.args.get('sndr', type=str)
    msg = request.args.get('msg', type=str)

    c = con.cursor()
    c.execute('INSERT INTO messages(recp, sndr, msg) VALUES(?, ?, ?)', (recp, sndr, msg))
    con.commit()
    c.close()
    return "OK"
