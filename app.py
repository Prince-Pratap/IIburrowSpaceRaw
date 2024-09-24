from flask import Flask, request, send_from_directory
from flask_socketio import SocketIO
import os

app = Flask(__name__)
socketio = SocketIO(app)

# Directory where files will be stored
sync_directory = 'sync_directory'
if not os.path.exists(sync_directory):
    os.makedirs(sync_directory)

@app.route('/')
def index():
    return send_from_directory('.', 'index.html')

@app.route('/upload', methods=['POST'])
def upload_file():
    if 'file' not in request.files:
        return 'No file part', 400
    file = request.files['file']
    if file.filename == '':
        return 'No selected file', 400
    file.save(os.path.join(sync_directory, file.filename))
    
    # Prepare the full file path to send to clients
    file_url = f'/sync/{file.filename}'
    
    # Notify other peers with the file path
    socketio.emit('file_uploaded', {'filename': file.filename, 'url': file_url})
    
    return 'File uploaded successfully', 200

# Route to serve the uploaded files
@app.route('/sync/<filename>')
def serve_file(filename):
    return send_from_directory(sync_directory, filename)

@socketio.on('connect')
def handle_connect():
    print('A user connected')

@socketio.on('new_peer')
def handle_new_peer(data):
    peer_id = data.get('id', None)
    if peer_id:
        print(f"New peer connected: {peer_id}")
        socketio.emit('new_peer', {'id': peer_id}, broadcast=True)
    else:
        print("Received new peer connection without an ID.")

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5000))
    # Allow unsafe Werkzeug usage in development (not recommended for production)
    socketio.run(app, host='0.0.0.0', port=port, allow_unsafe_werkzeug=True)
