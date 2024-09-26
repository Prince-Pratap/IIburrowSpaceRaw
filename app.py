from flask import Flask, request, send_from_directory
from flask_socketio import SocketIO
import os
import requests

app = Flask(__name__)
socketio = SocketIO(app)

# Directory where files will be stored on Render
sync_directory = 'sync_directory'
if not os.path.exists(sync_directory):
    os.makedirs(sync_directory)

# URL for the local server (accessible via ngrok)
local_server_url = 'https://42bd-117-250-76-240.ngrok-free.app'  # Update with your ngrok URL

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

    # Save the file on the Render server
    file.save(os.path.join(sync_directory, file.filename))
    
    # Send the file to the local server via ngrok
    try:
        with open(os.path.join(sync_directory, file.filename), 'rb') as f:
            files = {'file': (file.filename, f)}
            response = requests.post(f"{local_server_url}/sync_file", files=files)
            if response.status_code == 200:
                print("File successfully synced with local server")
            else:
                print(f"Failed to sync with local server: {response.status_code}")
    except Exception as e:
        print(f"Error notifying local server: {e}")
    
    # Notify other peers with the file path
    socketio.emit('file_uploaded', {'filename': file.filename, 'url': f'/sync/{file.filename}'})
    
    return 'File uploaded successfully', 200

# Route to serve the uploaded files
@app.route('/sync/<filename>')
def serve_file(filename):
    return send_from_directory(sync_directory, filename)

@app.route('/download/<filename>', methods=['GET'])
def download_file(filename):
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
    app.run(debug=True, allow_unsafe_werkzeug=True)
