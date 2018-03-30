from flask import Flask, render_template, request, make_response
import pickle
import base64

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        data = request.form['data']
    elif 'data' in request.cookies:
        data = base64.b64decode(request.cookies['data'])
        data = pickle.loads(data)
    else:
        data = ""
    
    data_s = base64.b64encode(pickle.dumps(data))

    res = make_response(render_template('index.html', data=data))
    res.set_cookie('data', data_s)
    
    return res
